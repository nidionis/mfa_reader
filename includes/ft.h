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

#define ERROR -1
#define EXTENTION_AVAILABLE ".mfa"

typedef struct s_data {
    size_t  size;
    void    *bin;
    void    *orgin;
} t_data;

typedef struct {
    const char *name;
    const unsigned char *signature;
    size_t signature_length;
} FileHeader;

extern const FileHeader headers_[];
extern const int HEADERS_COUNT;

int	get_rgba(int t, int r, int g, int b);
int load_file(t_data *data, char *path);
int is_header(unsigned char *data, size_t pos, const FileHeader *header);
const FileHeader* find_header(unsigned char *data, size_t data_size);

#endif
