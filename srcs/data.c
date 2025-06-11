//
// Created by nidionis on 11/06/25.
//

#include "ft.h"

int load_file(t_data *data, char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error opening file");
        return ERROR;
    }

    fseek(file, 0, SEEK_END);
    data->size = ftell(file);
    rewind(file);

    data->bin = malloc(data->size);
    if (!data->bin) {
        fclose(file);
        return ERROR;
    }

    size_t read_count = fread(data->bin, 1, data->size, file);
    if (read_count != data->size) {
        perror("File read error");
        free(data->bin);
        fclose(file);
        return ERROR;
    }

    data->orgin = data->bin;
    fclose(file);
    return 0;
}

//int load_file(t_data *data, char *path) {
//    FILE *file = fopen(path, "rb");
//    if (!file) {
//        perror("Error opening file");
//        return ERROR;
//    }
//    data->bin = malloc(data->size);
//    if (!data->bin) {
//        fclose(file);
//        return ERROR;
//    }
//    fread(data->bin, data->size, 1, file);
//    data->orgin = data->bin;
//    return 0;
//}
