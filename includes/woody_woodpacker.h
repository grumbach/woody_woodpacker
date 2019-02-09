/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/06 07:33:12 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_WOODPACKER_H
# define WOODY_WOODPACKER_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <linux/elf.h>

# define __unused				__attribute__((unused))
# define __noreturn				__attribute__((noreturn))
# define __warn_unused_result	__attribute__((warn_unused_result))


/*
** XTEA encryption
*/

void	encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
void	decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);

/*
** safe file accessing
*/

void			*safe(const Elf64_Off offset, const size_t size);
bool			read_file(const char *filename);
bool			free_file(void);

/*
** endian management
*/

void			endian_big_mode(bool is_big_endian);
uint16_t		endian_2(uint16_t n);
uint32_t		endian_4(uint32_t n);
uint64_t		endian_8(uint64_t n);

/*
** errors
*/

void			fatal(const char * const message);
void			warn(const char * const message);

#endif
