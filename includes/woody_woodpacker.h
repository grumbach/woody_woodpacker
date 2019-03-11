/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/11 18:02:05 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_WOODPACKER_H
# define WOODY_WOODPACKER_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <linux/elf.h>

# include "compiler_utils.h"
# include "elf64.h"

# define OUTPUT_FILENAME		"woody"

/*
** managed formats
*/

typedef enum			e_file_fmt
{
	FMT_ELF64,
	// FMT_ELF32,
	// FMT_MACHO64,
	// FMT_MACHO32,
	// FMT_PE64,
	// FMT_PE32,
	FMT_SIZE                    // alway last
}						t_file_fmt;

/*
** f_identifier
**   returns true if identifies valid file format, sets endian globally
** f_packer
**   encrypts entry section, adds decryption routine before original entry
*/

typedef bool			(*f_identifier)(void);
typedef bool			(*f_packer)(__nonull void *clone, size_t original_filesize);

typedef struct			s_format
{
	f_identifier		format_identifier;
	f_packer			packer;
}						t_format;

/*
** encryption
*/

void			encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void			decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

/*
** safe file accessing
*/

__warn_unused_result
void			*safe(const Elf64_Off offset, const size_t size);
size_t			read_file(const char *filename);
void			free_file(void);

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
