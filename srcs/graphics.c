//
// Created by nidionis on 11/06/25.
//

#include "ft.h"

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
