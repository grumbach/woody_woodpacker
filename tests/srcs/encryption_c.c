/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encryption_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 15:49:51 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/11 18:17:32 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xtea_test.h"

/*
** XTEA encryption
*/

void	encipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);
void	decipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);

// TODO translate encrypt and decrypt to ASM!

void	c_encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)
{
	while (size >= 8)
	{
		encipher(num_rounds, (uint32_t*)data, key);
		data += 8;
		size -= 8;
	}
}

void	c_decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)
{
	while (size >= 8)
	{
		decipher(num_rounds, (uint32_t*)data, key);
		data += 8;
		size -= 8;
	}
}
