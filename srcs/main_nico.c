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
#define OFFSET 4

//const int HEADERS_COUNT = sizeof(headers_) / sizeof(headers_[0]);

//int main(int argc, char **argv)
//{
//    t_data data;
//
//    if (argc != 2) {
//        printf("Usage: %s <file.mfa>\n", argv[0]);
//        return 1;
//    }
//    load_file(&data, argv[1]);
//
//    const FileHeader *found = find_header(data.bin, data.size);
//    if (found) {
//        printf("Found header: %s\n", found->name);
//    } else {
//        printf("No known header found\n");
//    }
//
////    for (size_t pos = 0; pos < data.size; pos += OFFSET) {
////        if (memcmp(data.bin + pos, "MFA ", 4) == 0) {
////            printf("MFA found at %zu\n", (void *)pos - data.orgin);
////        }
////        //fseek(data.bin, pos, SEEK_SET);
////    }
//    return 1;
//}
