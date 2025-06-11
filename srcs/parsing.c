//
// Created by nidionis on 11/06/25.
//

#include "ft.h"

int is_extention_ok(const char *filepath, const char *extention) {
    if (!filepath) {
        perror("[is_extention_ok] filepath is null]");
        return -1;
    }
    if (!extention) {
        perror("warning: [is_extention_ok] accepting any extention");
    }
    char *ext = strstr(filepath, extention);
    if (ext) {
        if (ext == filepath + strlen(filepath) - strlen(extention)) {
            return 1;
        }
    }
    return 0;
}

uint8_t* read_file_data(const char *filepath, unsigned long *filesize) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        perror("Error opening file");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *filesize = ftell(f);
    rewind(f);
    uint8_t *data = malloc(*filesize);
    if (!data) {
        perror("Memory allocation error");
        fclose(f);
        return NULL;
    }

    size_t bytes_read = fread(data, 1, *filesize, f);
    fclose(f);
    if (bytes_read != *filesize) {
        perror("Error reading file");
        free(data);
        return NULL;
    }
    return data;
}
