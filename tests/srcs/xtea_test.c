/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xtea_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/14 15:55:58 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xtea_test.h"

# define BUFF_SIZE			1023

void	randomize_mem(char *buf, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buf[i] = random();
}

int		main(__unused int ac, __unused char **av)
{
	srandom(time(NULL));
	uint32_t	r = random() % 32 + 32; // between 32 and 64
	uint32_t	key[4] = {random(), random(), random(), random()};
	char		buffer[BUFF_SIZE]; randomize_mem(buffer, BUFF_SIZE);
	char		buffer2[BUFF_SIZE]; memcpy(buffer2, buffer, BUFF_SIZE);

	printf("=== rounds: %u key: %x%x%x%x ===\n", r, key[0], key[1], key[2], key[3]);
	printf("- xtea.c - " " cksum: [%d]\n\n", cksum(buffer, BUFF_SIZE));
	printf("- xtea.s - " " cksum: [%d]\n\n", cksum(buffer2, BUFF_SIZE));

	c_encrypt(r, buffer, key, BUFF_SIZE);
	printf("- xtea.c - " "encrypt cksum: [%d]\n\n", cksum(buffer, BUFF_SIZE));
	encrypt(r, buffer2, key, BUFF_SIZE);
	printf("- xtea.s - " "encrypt cksum: [%d]\n\n", cksum(buffer2, BUFF_SIZE));

	assert(memcmp(buffer, buffer2, BUFF_SIZE) == 0);

	c_decrypt(r, buffer, key, BUFF_SIZE);
	printf("- xtea.c - " "decrypt cksum: [%d]\n\n", cksum(buffer, BUFF_SIZE));
	decrypt(r, buffer2, key, BUFF_SIZE);
	printf("- xtea.s - " "decrypt cksum: [%d]\n\n", cksum(buffer2, BUFF_SIZE));

	assert(memcmp(buffer, buffer2, BUFF_SIZE) == 0);

	return (0);
}
