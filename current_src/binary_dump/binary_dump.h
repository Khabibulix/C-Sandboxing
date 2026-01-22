#ifndef BINARY_DUMP_H
#define BINARY_DUMP_H

#include <stdint.h>

__attribute__((packed))
struct header {
    uint8_t magic[4];
    uint8_t version;
    uint8_t flags[1];
    uint16_t header_size;
    uint32_t payload_size;
};

#endif
