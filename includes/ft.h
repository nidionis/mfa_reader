/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:56:57 by benpicar          #+#    #+#             */
/*   Updated: 2025/06/10 16:57:33 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef FT_H
# define FT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>

#define ERROR -1
#define EXTENTION_AVAILABLE ".mfa"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../MLX42/include/MLX42/MLX42.h"
#include <ft.h>

typedef struct s_image
{
    uint16_t width;
    uint16_t height;
    size_t pixel_count;
    uint8_t *rgb_data;
    char filename[64];
}	t_image;

typedef struct s_viewer_ctx
{
    t_image *images;
    int total;
    int current;
    mlx_t *mlx;
    mlx_image_t *mlx_img;
    int zoom;
}	t_viewer_ctx;

#define SCALE_5_TO_8(x) ((x) * 255 / 31)
#define MAX_IMAGES 1000

typedef struct s_data {
    size_t  size;
    void    *bin;
    void    *orgin;
} t_data;

//typedef struct {
//    const char *name;
//    const unsigned char *signature;
//    size_t signature_length;
//} FileHeader;
//
//extern const FileHeader headers_[];
//extern const int HEADERS_COUNT;
//
//int	get_rgba(int t, int r, int g, int b);
//int load_file(t_data *data, char *path);
//int is_header(unsigned char *data, size_t pos, const FileHeader *header);
//const FileHeader* find_header(unsigned char *data, size_t data_size);

uint8_t* read_file_data(const char *filepath, unsigned long *filesize);
int is_extention_ok(const char *filepath, const char *extention);
void convert_16bit_pixel_to_rgb(uint16_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);
void convert_pixel_to_rgb_data(t_image *img, size_t p, uint16_t pixel);
int initialize_image(t_image *img, int count, uint16_t width, uint16_t height);
int extract_24bit_image(uint8_t *data, unsigned long filesize, t_image *images, int count, unsigned long i);
int extract_16bit_image(uint8_t *data, unsigned long filesize, t_image *images, int count, unsigned long i);
int show_image_zoom3_with_mlx(t_image *img);
void	draw_zoomed_image(t_viewer_ctx *ctx);
void	key_hook(mlx_key_data_t keydata, void *param);
void	my_close_win(void *param);
int show_images_zoomable_with_mlx(t_image *images, int total, int zoom);

#endif
