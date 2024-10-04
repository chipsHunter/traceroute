//
// Created by hvorostok on 02.10.2024.
//

#ifndef WORKING_TRACING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>

#define MAX_HOPS 30
#define TIMEOUT 1
#define DEST_PORT 33434  // Стартовый порт для отправки UDP пакетов
#define MAX_ATTEMPTS 3     // Число пакетов для каждого TTL

void trace_udp(const char *host);

#endif //WORKING_TRACING_H
