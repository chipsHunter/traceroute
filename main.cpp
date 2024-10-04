#include <cstdio>
#include <cstdlib>
#include "tracing.h"

void print_help() {
    printf("Usage: hvtraceroute [options] <hostname>\n");
    printf("Options:\n");
    printf("  -q <attempts>   Number of attempts per hop (default: 3)\n");
    printf("  -h <max_hop>    Maximum number of hops (default: 30)\n");
    printf("  -f <ttl>        Time to live (default: 1)\n");
    printf("  --help          Display this help message\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1], "--help") == 0) {
        print_help();
        exit(0);
    }

    int option;
    struct trace_options options;
    initialize_options(&options);

    // Использование getopt для обработки аргументов
    while ((option = getopt(argc, argv, "q:f:h:")) != -1) {
        switch (option) {
            case 'q':
                options.attempts = atoi(optarg);
                break;
            case 'h':
                options.MAX_HOP = atoi(optarg);
                break;
            case 'f':
                options.TTL = atoi(optarg);
                break;
            default:
                break;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected hostname after options\n");
        exit(EXIT_FAILURE);
    }
    const char* host_name = argv[optind];
    sethost(options.HOST, host_name);

    trace_udp(&options);
    return 0;
}