/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2018/11/04 18:33:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XTEA_TEST_H
# define XTEA_TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define __unused __attribute__((unused))

// xtea.c
void	encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
void	decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);

// xtea.s
void	s_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
void	s_decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);

#endif
