//
// Created by nidionis on 11/06/25.
//

#include "ft.h"

void convert_16bit_pixel_to_rgb(uint16_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) {
    *b = SCALE_5_TO_8((pixel >> 10) & 0x1F);
    *g = SCALE_5_TO_8((pixel >> 5) & 0x1F);
    *r = SCALE_5_TO_8((pixel >> 0) & 0x1F);
}

void convert_pixel_to_rgb_data(t_image *img, size_t p, uint16_t pixel) {
    uint8_t r, g, b;
    convert_16bit_pixel_to_rgb(pixel, &r, &g, &b);
    img->rgb_data[3*p]     = r;
    img->rgb_data[3*p + 1] = g;
    img->rgb_data[3*p + 2] = b;
}

int initialize_image(t_image *img, int count, uint16_t width, uint16_t height) {
    img->width = width;
    img->height = height;
    img->pixel_count = width * height;

    snprintf(img->filename, sizeof(img->filename), "image_%d_%dx%d.ppm", count, width, height);

    img->rgb_data = malloc(img->pixel_count * 3);
    if (!img->rgb_data) {
        perror("Error allocating RGB data");
        return -1;
    }

    return 0;
}

int extract_24bit_image(uint8_t *data, unsigned long filesize, t_image *images, int count, unsigned long i) {
    uint16_t width  = data[i - 4] | (data[i - 3] << 8);
    uint16_t height = data[i - 2] | (data[i - 1] << 8);

    if (width == 0 || height == 0)
        return -1;

    size_t pixel_count = width * height;
    size_t pixel_bytes = pixel_count * 3;
    unsigned long pixel_offset = i + 16;

    if (pixel_offset + pixel_bytes > filesize)
        return -1;

    t_image *img = &images[count];
    img->width = width;
    img->height = height;
    img->pixel_count = pixel_count;
    snprintf(img->filename, sizeof(img->filename), "image_%d_%dx%d.ppm", count, width, height);

    img->rgb_data = malloc(pixel_count * 3);
    if (!img->rgb_data) {
        perror("Error allocating RGB data");
        return -1;
    }

    for (size_t p = 0; p < pixel_count; ++p) {
        uint8_t b = data[pixel_offset + 3*p];
        uint8_t g = data[pixel_offset + 3*p + 1];
        uint8_t r = data[pixel_offset + 3*p + 2];

        img->rgb_data[3*p]     = r;
        img->rgb_data[3*p + 1] = g;
        img->rgb_data[3*p + 2] = b;
    }

    return pixel_offset + pixel_bytes - 1;
}


int extract_16bit_image(uint8_t *data, unsigned long filesize, t_image *images, int count, unsigned long i) {
    uint16_t width  = data[i - 4] | (data[i - 3] << 8);
    uint16_t height = data[i - 2] | (data[i - 1] << 8);
    if (width == 0 || height == 0)
        return -1;
    size_t pixel_count = width * height;
    size_t pixel_bytes = pixel_count * 2;
    unsigned long pixel_offset = i + 16;
    if (pixel_offset + pixel_bytes > filesize)
        return -1;
    t_image *img = &images[count];
    if (initialize_image(img, count, width, height) != 0) {
        return -1;
    }
    for (size_t p = 0; p < pixel_count; ++p) {
        uint16_t pix = data[pixel_offset + 2*p] | (data[pixel_offset + 2*p + 1] << 8);
        convert_pixel_to_rgb_data(img, p, pix);
    }
    return pixel_offset + pixel_bytes - 1;  // Return new index
}
