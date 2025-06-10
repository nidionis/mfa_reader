/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:19:16 by benpicar          #+#    #+#             */
/*   Updated: 2025/06/10 17:54:45 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

// int	main(int argc, char **argv)
// {
// 	if (argc != 2)
// 	{
// 		ft_putstr("Error: Invalid number of arguments\n");
// 		return (1);
// 	}
// 	if (!ft_is_valid_input(argv[1]))
// 	{
// 		ft_putstr("Error: Invalid input\n");
// 		return (1);
// 	}
// 	ft_process_input(argv[1]);
// 	return (0);
// }

#define PNG_HEADER "\x89PNG\r\n\x1a\n"
#define PNG_HEADER_SIZE 8
#define JPEG_HEADER "\xFF\xD8\xFF"
#define JPEG_HEADER_SIZE 3
#define JPEG_FOOTER "\xFF\xD9"
#define JPEG_FOOTER_SIZE 2

int main_ben(int argc, char **argv) {
    (void)argc;
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    unsigned char *data = malloc(filesize);
    if (!data) {
        fclose(file);
        return 1;
    }
    fread(data, 1, filesize, file);
    fclose(file);

// Cherche le header PNG
    for (long i = 0; i < filesize - PNG_HEADER_SIZE; ++i) {
        if (memcmp(data + i, PNG_HEADER, PNG_HEADER_SIZE) == 0) {
// Trouvé ! Cherche la fin du PNG (IEND chunk)
            for (long j = i + PNG_HEADER_SIZE; j < filesize - 8; ++j) {
                if (memcmp(data + j, "IEND", 4) == 0) {
                    long end = j + 8; // IEND chunk + CRC
                    FILE *out = fopen("extracted.png", "wb");
                    if (out) {
                        fwrite(data + i, 1, end - i, out);
                        fclose(out);
                        printf("Image PNG extraite : extracted.png\n");
                    }
                    free(data);
                    return 0;
                }
            }
        }
    }
    for (long i = 0; i < filesize - JPEG_HEADER_SIZE; ++i) {
        if (memcmp(data + i, JPEG_HEADER, JPEG_HEADER_SIZE) == 0) {
// Trouvé ! Cherche la fin du JPEG
            for (long j = i + JPEG_HEADER_SIZE; j < filesize - JPEG_FOOTER_SIZE; ++j) {
                if (memcmp(data + j, JPEG_FOOTER, JPEG_FOOTER_SIZE) == 0) {
                    long end = j + JPEG_FOOTER_SIZE;
                    FILE *out = fopen("extracted.jpg", "wb");
                    if (out) {
                        fwrite(data + i, 1, end - i, out);
                        fclose(out);
                        printf("Image JPEG extraite : extracted.jpg\n");
                    }
                    free(data);
                    return 0;
                }
            }
        }
    }
    printf("Aucune image PNG trouvée dans le fichier.\n");
    free(data);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <file.mfa>\n", argv[0]);
        return 1;
    }
    get_rgba(0,0,0,0);
    return 1;
}
