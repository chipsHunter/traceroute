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

#define TIMEOUT 1
#define DEST_PORT 33434  // Стартовый порт для отправки UDP пакетов


struct trace_options {
    char* HOST;
    int MAX_HOP;
    int attempts;
    int TTL;
};

void initialize_options(struct trace_options* options);
void trace_udp(struct trace_options* options);

#endif //WORKING_TRACING_H
