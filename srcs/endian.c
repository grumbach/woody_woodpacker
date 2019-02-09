/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 23:54:56 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/03 18:24:28 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static bool			big_endian = false;

void				endian_big_mode(bool is_big_endian)
{
	big_endian = is_big_endian;
}

uint16_t			endian_2(uint16_t n)
{
	if (big_endian)
		return ((n >> 8) | (n << 8));
	return (n);
}

uint32_t			endian_4(uint32_t n)
{
	if (big_endian)
		return ((n >> 24) | ((n & 0xff0000) >> 8) | \
			((n & 0xff00) << 8) | (n << 24));
	return (n);
}

uint64_t			endian_8(uint64_t n)
{
	if (big_endian)
		return ((n & 0xff00000000000000) >> 56 \
			| (n & 0x00ff000000000000) >> 40 \
			| (n & 0x0000ff0000000000) >> 24 \
			| (n & 0x000000ff00000000) >> 8 \
			| (n & 0x00000000ff000000) << 8 \
			| (n & 0x0000000000ff0000) << 24 \
			| (n & 0x000000000000ff00) << 40 \
			| (n & 0x00000000000000ff) << 56);
	return (n);
}
