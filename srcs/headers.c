//
// Created by nidionis on 11/06/25.
//
#include "ft.h"

int is_header(unsigned char *data, size_t pos, const FileHeader *header) {
    return memcmp(data + pos, header->signature, header->signature_length) == 0;
}

const FileHeader* find_header(unsigned char *data, size_t data_size) {
    for (size_t pos = 0; pos < data_size; ++pos) {
        for (int i = 0; i < HEADERS_COUNT; ++i) {
            if (is_header(data, pos, &headers_[i])) {
                return &headers_[i];
            }
        }
    }
    return NULL;
}
