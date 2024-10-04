#include <cstdio>
#include <cstdlib>
#include "tracing.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(1);
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