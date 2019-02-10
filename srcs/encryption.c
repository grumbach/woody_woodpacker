/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encryption.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 15:49:51 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/10 15:52:59 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

/*
** XTEA encryption
*/

void	encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
void	decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);

// TODO translate encrypt and decrypt to ASM!

void	encrypt(char *data, size_t size, uint num_rounds, uint32_t const key[4])
{
	char	buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	while (size >= 8)
	{
		encipher(num_rounds, (uint32_t*)data, key);
		data += 8;
		size -= 8;
	}
	if (size)
	{
		for (size_t i = 0; i < size; i++)
			buffer[i] = data[i];
		encipher(num_rounds, (uint32_t*)buffer, key);
		for (size_t i = 0; i < size; i++)
			data[i] = buffer[i];
	}
}

void	decrypt(char *data, size_t size, uint num_rounds, uint32_t const key[4])
{
	char	buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	while (size >= 8)
	{
		decipher(num_rounds, (uint32_t*)data, key);
		data += 8;
		size -= 8;
	}
	if (size)
	{
		for (size_t i = 0; i < size; i++)
			buffer[i] = data[i];
		decipher(num_rounds, (uint32_t*)buffer, key);
		for (size_t i = 0; i < size; i++)
			data[i] = buffer[i];
	}
}
