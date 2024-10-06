#include "tracing.h"

void sethost(char* host, const char* name) {
    strcpy(host, name);
    *(host + strlen(host)) = '\0';
}

void initialize_options(struct trace_options* options) {
    options->MAX_HOP = 30;
    options->attempts = 3;
    options->TTL = 1;
    options->HOST = (char*)calloc(128, sizeof(char));
}
// Функция для проверки, является ли полученное ICMP-сообщение "Destination Unreachable"
int is_destination_unreachable(unsigned char *buf, int len) {
    struct ip *ip_header = (struct ip *) buf;
    int ip_header_len = ip_header->ip_hl * 4;

    struct icmp *icmp_header = (struct icmp *) (buf + ip_header_len);

    return (icmp_header->icmp_type == ICMP_DEST_UNREACH && icmp_header->icmp_code == ICMP_PORT_UNREACH);
}

// Функция для вычисления разницы во времени в миллисекундах
double time_diff(struct timeval *start, struct timeval *end) {
    return (double) (end->tv_sec - start->tv_sec) * 1000.0 +
           (double) (end->tv_usec - start->tv_usec) / 1000.0;
}

int open_socket(int domain, int type, int protocol) {
    int sockt = socket(domain, type, protocol);
    if (sockt < 0) {
        perror("socket");
        exit(1);
    }
    return sockt;
}

void set_socket_timeout(int *sockt, struct timeval *timeout) {
    if (setsockopt(*sockt, SOL_SOCKET, SO_RCVTIMEO, timeout, sizeof(*timeout)) < 0) {
        perror("setsockopt");
        exit(1);
    }
}

void set_hints_for_host_searching(struct addrinfo *hints, int ipv) {
    memset(hints, 0, sizeof(hints));
    if (ipv == 4)
        hints->ai_family = AF_INET;
    else
        hints->ai_family = AF_INET6;
    hints->ai_socktype = SOCK_DGRAM;
    hints->ai_protocol = IPPROTO_UDP;
}

int recvfrom_s(int *icmp_socket, char* recv_buffer, socklen_t* addr_len, sockaddr_in* recv_addr) {
    int recv_len = recvfrom(*icmp_socket, recv_buffer, RECV_BUFFER, 0, (struct sockaddr *) recv_addr,
                            addr_len);
    if (recv_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("* ");
        } else {
            perror("recvfrom");
        }
    }
    return recv_len;
}

void setsockopt_s(const int *udp_socket, const int *ttl) {
    if (setsockopt(*udp_socket, IPPROTO_IP, IP_TTL, ttl, sizeof(*ttl)) < 0) {
        perror("setsockopt");
        exit(1);
    }
}

void sendto_s(const int *udp_socket, const sockaddr_in *dest_addr, const char *send_buffer) {
    if (sendto(*udp_socket, send_buffer, sizeof(send_buffer), 0, (struct sockaddr *)dest_addr,
               sizeof(*dest_addr)) < 0) {
        perror("sendto");
        exit(1);
    }
}

void print_attempt(int print_addr, timeval &start_time, timeval &end_time, sockaddr_in &recv_addr) {
    double elapsed_time = time_diff(&start_time, &end_time);

    // Выводим IP адрес хопа только при первом успешном ответе
    if (print_addr == 1) {
        printf("%s ", inet_ntoa(recv_addr.sin_addr));
    }

    printf("%.2f ms ", elapsed_time);
}

int trace(int *udp_socket, int *icmp_socket, struct sockaddr_in* dest_addr, int *ttl, int print_addr) {
    struct timeval start_time, end_time;
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);

    char send_buffer[64] = "traceroute test";  // Тело UDP пакета
    char* recv_buffer = (char*)calloc(RECV_BUFFER, sizeof(char));

    setsockopt_s(udp_socket, ttl);
    gettimeofday(&start_time, NULL);
    sendto_s(udp_socket, dest_addr, send_buffer);
    int recv_len = recvfrom_s(icmp_socket, recv_buffer, &addr_len, &recv_addr);
    gettimeofday(&end_time, NULL);

    if (recv_len >= 0) {
        // Вычисляем время отклика
        print_attempt(print_addr, start_time, end_time, recv_addr);

        if (is_destination_unreachable((unsigned char *) recv_buffer, recv_len)) {
            printf("\nReached target: %s\n", inet_ntoa(recv_addr.sin_addr));
            return 1;
        }
    }
    return 0;
}

// Функция для выполнения трассировки
void trace_udp(struct trace_options* options) {
    struct sockaddr_in dest_addr;
    int ttl = options->TTL;

    // Создание UDP сокета для отправки пакетов
    int udp_socket = open_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int icmp_socket = open_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct timeval timeout = {TIMEOUT, 0};
    set_socket_timeout(&icmp_socket, &timeout);

    // Используем getaddrinfo для получения IP-адреса хоста
    struct addrinfo hints, *res;
    set_hints_for_host_searching(&hints, 4);

    if (getaddrinfo(options->HOST, NULL, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    // Копируем информацию об адресе в структуру sockaddr_in
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr = ((struct sockaddr_in *) res->ai_addr)->sin_addr;
    dest_addr.sin_port = htons(DEST_PORT);  // Указываем порт

    freeaddrinfo(res);  // Освобождаем память, выделенную под результат getaddrinfo

    int tracerouting = 1;
    // Цикл по увеличению TTL для отслеживания хопов
    for ( ; ttl <= options->MAX_HOP && tracerouting; ttl++) {

        printf("%2d ", ttl);

        for (int attempt = 0; attempt < options->attempts; attempt++) {
            if((trace(&udp_socket, &icmp_socket, &dest_addr, &ttl, attempt == 0)) == 1) {
                tracerouting = 0;
                break;
            }
        }

        printf("\n");
    }

    // Закрываем сокеты
    close(udp_socket);
    close(icmp_socket);
}