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

int	get_rgba(int t, int r, int g, int b);
int load_file(t_data *data, char *path);

#endif
