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

#include "ft.h"

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
        if ((data[i] == 0x06 || data[i] == 0x07) && data[i+1] == 0x10) {
            long new_index = extract_16bit_image(data, filesize, images, count, i);
            if (new_index != -1) {
                count++;
                i = new_index;
            }
        }
		else if (data[i] == 0x04 && data[i+1] == 0x10) {
            long new_index = extract_24bit_image(data, filesize, images, count, i);
            if (new_index != -1) {
                count++;
                i = new_index;
            }
        }
    }
    free(data);
    return (count);
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

