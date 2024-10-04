//
// Created by hvorostok on 01.10.2024.
//

#include <cstdio>
#include <cstdlib>
#include "tracing.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(1);
    }

    trace_udp(argv[1]);
    return 0;
}
