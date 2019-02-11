/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xtea_test.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/11 18:12:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XTEA_TEST_H
# define XTEA_TEST_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define __unused __attribute__((unused))

// xtea.s
void	encipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);
void	decipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);

// encrypt.c
void	c_encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void	c_decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

// encrypt.s
void	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

// checksum
uint16_t		cksum(__unused const void *buffer, __unused size_t size);

#endif
