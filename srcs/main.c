/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:51:47 by benpicar          #+#    #+#             */
/*   Updated: 2025/06/11 17:37:25 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

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
static uint8_t* read_file_data(const char *filepath, unsigned long *filesize) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        perror("Error opening file");
        return NULL;
    }

    // Determine file size
    fseek(f, 0, SEEK_END);
    *filesize = ftell(f);
    rewind(f);

    // Allocate memory for file data
    uint8_t *data = malloc(*filesize);
    if (!data) {
        perror("Memory allocation error");
        fclose(f);
        return NULL;
    }

    // Read file content
    size_t bytes_read = fread(data, 1, *filesize, f);
    fclose(f);

    if (bytes_read != *filesize) {
        perror("Error reading file");
        free(data);
        return NULL;
    }

    return data;
}

int extract_images(const char *filepath, t_image *images, int max_images) {
    // Validate file extension
    if (!is_extention_ok(filepath, EXTENTION_AVAILABLE)) {
        perror("Invalid file extension");
        return -1;
    }

    // Read file data
    unsigned long filesize;
    uint8_t *data = read_file_data(filepath, &filesize);
    if (!data) {
        return -1;
    }

    int count = 0;

    for (unsigned long i = 4; i < filesize - 10 && count < max_images; ++i)
	{
        if ((data[i] == 0x06 || data[i] == 0x07)  && data[i+1] == 0x10)
		{
            uint16_t width  = data[i - 4] | (data[i - 3] << 8);
            uint16_t height = data[i - 2] | (data[i - 1] << 8);
            if (width == 0 || height == 0)
			{
    			continue;
			}
			size_t pixel_count = width * height;
            size_t pixel_bytes = pixel_count * 2;
            unsigned long pixel_offset = i + 16;

            if (pixel_offset + pixel_bytes > filesize)
				continue;

            t_image *img = &images[count];
            img->width = width;
            img->height = height;
            img->pixel_count = pixel_count;
            snprintf(img->filename, sizeof(img->filename), "image_%d_%dx%d.ppm", count, width, height);

            img->rgb_data = malloc(pixel_count * 3);
            if (!img->rgb_data)
			{
                perror("Erreur allocation RGB");
                break;
            }

            for (size_t p = 0; p < pixel_count; ++p)
			{
                uint16_t pix = data[pixel_offset + 2*p] | (data[pixel_offset + 2*p + 1] << 8);
                uint8_t b = SCALE_5_TO_8((pix >> 10) & 0x1F);
                uint8_t g = SCALE_5_TO_8((pix >> 5) & 0x1F);
                uint8_t r = SCALE_5_TO_8((pix >> 0) & 0x1F);

                img->rgb_data[3*p]     = r;
                img->rgb_data[3*p + 1] = g;
                img->rgb_data[3*p + 2] = b;
            }

            count++;
            i = pixel_offset + pixel_bytes - 1;
        }
		else if (data[i] == 0x04 && data[i+1] == 0x10)
		{
			uint16_t width  = data[i - 4] | (data[i - 3] << 8);
			uint16_t height = data[i - 2] | (data[i - 1] << 8);

			if (width == 0 || height == 0)
			{
				continue;
			}
			size_t pixel_count = width * height;
			size_t pixel_bytes = pixel_count * 3;
			long pixel_offset = i + 16;

			if (pixel_offset + pixel_bytes > filesize)
			{
				continue;
			}
			t_image *img = &images[count];
			img->width = width;
			img->height = height;
			img->pixel_count = pixel_count;
			snprintf(img->filename, sizeof(img->filename), "image_%d_%dx%d.ppm", count, width, height);

			img->rgb_data = malloc(pixel_count * 3);
			if (!img->rgb_data)
			{
				perror("Erreur allocation RGB");
				break;
			}

			for (size_t p = 0; p < pixel_count; ++p)
			{
				uint8_t b = data[pixel_offset + 3*p];
				uint8_t g = data[pixel_offset + 3*p + 1];
				uint8_t r = data[pixel_offset + 3*p + 2];

				img->rgb_data[3*p]     = r;
				img->rgb_data[3*p + 1] = g;
				img->rgb_data[3*p + 2] = b;
			}

			count++;
			i = pixel_offset + pixel_bytes - 1;
		}

    }

    free(data);
    return (count);
}

int show_image_zoom3_with_mlx(t_image *img)
{
    if (!img || !img->rgb_data)
        return 1;

    int zoom = 10;
    int win_w = img->width * zoom;
    int win_h = img->height * zoom;

    mlx_t *mlx = mlx_init(win_w, win_h, "Image Zoom x3", false);
    if (!mlx)
        return 1;

    mlx_image_t *mlx_img = mlx_new_image(mlx, win_w, win_h);
    if (!mlx_img)
        return 1;

    for (size_t y = 0; y < img->height; ++y) {
        for (size_t x = 0; x < img->width; ++x) {
            size_t p = y * img->width + x;
            uint8_t r = img->rgb_data[3*p];
            uint8_t g = img->rgb_data[3*p + 1];
            uint8_t b = img->rgb_data[3*p + 2];
            uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;
            for (int dy = 0; dy < zoom; ++dy) {
                for (int dx = 0; dx < zoom; ++dx) {
                    size_t zx = x * zoom + dx;
                    size_t zy = y * zoom + dy;
                    ((uint32_t*)mlx_img->pixels)[zy * win_w + zx] = color;
                }
            }
        }
    }

    mlx_image_to_window(mlx, mlx_img, 0, 0);
    mlx_loop(mlx);
    return 0;
}

void	draw_zoomed_image(t_viewer_ctx *ctx)
{
    t_image	*img = &ctx->images[ctx->current];
    int		win_w = img->width * ctx->zoom;
    int		win_h = img->height * ctx->zoom;

    if (ctx->mlx_img)
        mlx_delete_image(ctx->mlx, ctx->mlx_img);

    mlx_set_window_size(ctx->mlx, win_w + 100, win_h + 100);

    ctx->mlx_img = mlx_new_image(ctx->mlx, win_w, win_h);
    if (!ctx->mlx_img)
        return;

    for (size_t y = 0; y < img->height; ++y) {
        for (size_t x = 0; x < img->width; ++x) {
            size_t	p = y * img->width + x;
            uint8_t	r = img->rgb_data[3*p];
            uint8_t	g = img->rgb_data[3*p + 1];
            uint8_t	b = img->rgb_data[3*p + 2];
            uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;
            for (int dy = 0; dy < ctx->zoom; ++dy) {
                for (int dx = 0; dx < ctx->zoom; ++dx) {
                    size_t	zx = x * ctx->zoom + dx;
                    size_t	zy = y * ctx->zoom + dy;
                    ((uint32_t*)ctx->mlx_img->pixels)[zy * win_w + zx] = color;
                }
            }
        }
    }
    mlx_image_to_window(ctx->mlx, ctx->mlx_img, 0, 0);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
    t_viewer_ctx *ctx = (t_viewer_ctx*)param;
    if (keydata.action != MLX_PRESS)
        return ;

    if (keydata.key == MLX_KEY_RIGHT)
	{
			ctx->current = (ctx->current + 1) % ctx->total;
            draw_zoomed_image(ctx);
    }
    if (keydata.key == MLX_KEY_LEFT)
	{
            ctx->current = (ctx->current - 1 + ctx->total) % ctx->total;
            draw_zoomed_image(ctx);
    }
    if (keydata.key == MLX_KEY_ESCAPE)
	{
        mlx_close_window(ctx->mlx);
    }
}

void	my_close_win(void *param)
{
	t_viewer_ctx	*ctx = (t_viewer_ctx*)param;
	
	if (ctx->mlx_img)
	{
        mlx_delete_image(ctx->mlx, ctx->mlx_img);
	}
	mlx_close_window(ctx->mlx);
}

int show_images_zoomable_with_mlx(t_image *images, int total, int zoom)
{
    if (total <= 0)
        return 1;

    t_viewer_ctx	ctx;
    
	ctx.images = images;
    ctx.total = total;
    ctx.current = 0;
    ctx.zoom = zoom;
	ctx.mlx_img = NULL;

    int win_w = images[0].width * zoom;
    int win_h = images[0].height * zoom;

    ctx.mlx = mlx_init(win_w, win_h, "Image", false);
    if (!ctx.mlx)
        return (1);

    draw_zoomed_image(&ctx);

	mlx_close_hook(ctx.mlx, my_close_win, &ctx);
    mlx_key_hook(ctx.mlx, key_hook, &ctx);
    mlx_loop(ctx.mlx);

    if (ctx.mlx_img)
	{
        mlx_delete_image(ctx.mlx, ctx.mlx_img);
	}
	mlx_terminate(ctx.mlx);
    return (0);
}

int main(int ac, char **av)
{
    t_image images[MAX_IMAGES];

	if (ac != 2)
	{
		fprintf(stderr, "Usage: %s <fichier>\n", av[0]);
		return (1);
	}
	int total = extract_images(av[1], images, MAX_IMAGES);
	if (total < 0)
	{
		fprintf(stderr, "Échec de l'extraction\n");
		return (1);
	}

	printf("%d image(s) extraites.\n", total);

	if (total > 0)
		show_images_zoomable_with_mlx(images, total, 1);
	
	for (int i = 0; i < total; ++i)
	{
		free(images[i].rgb_data);
	}
	return (0);
}

