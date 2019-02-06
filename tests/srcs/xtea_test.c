/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/11/04 20:39:39 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xtea_test.h"
#include <time.h>

int		main(__unused int ac, __unused char **av)
{
	srandom(time(NULL));
	uint32_t	r = random() % 32 + 32; // between 32 and 64
	uint32_t	key[4] = {random(), random(), random(), random()};

	printf("=== rounds: %u key: %x%x%x%x ===\n", r, key[0], key[1], key[2], key[3]);

	uint64_t	a = random();
	uint64_t	b = a; encipher(r, (uint32_t*)&b, key);
	uint64_t	c = b; decipher(r, (uint32_t*)&c, key);

	puts("---------- wiki xtea.c ---------");
	printf("%08lx (x)-> %08lx (-x)-> %08lx\n\n", a, b, c);

	b = a; s_encipher(r, (uint32_t*)&b, key);
	c = b; s_decipher(r, (uint32_t*)&c, key);

	puts("---------- woody xtea.s --------");
	printf("%08lx (x)-> %08lx (-x)-> %08lx\n\n", a, b, c);
	
	return (0);
}
