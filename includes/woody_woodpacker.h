/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/02 18:44:38 by agrumbac         ###   ########.fr       */
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

# define __unused				__attribute__((unused))
# define __noreturn				__attribute__((noreturn))
# define __warn_unused_result	__attribute__((warn_unused_result))
# define __nonull				__attribute__((nonnull))

# define OUTPUT_FILENAME		"woody"

/*
** f_identifier
**   checks file format
*/

typedef bool	(*f_identifier)();
typedef bool	(*f_packer)();

typedef enum	e_file_fmt
{
	FMT_ELF64,
	FMT_ELF32,
	FMT_MACHO64,
	FMT_MACHO32,
	FMT_PE64,
	FMT_PE32,
	FMT_SIZE                    // alway last
}				t_file_fmt;

typedef struct			s_format
{
	f_identifier		format_identifier;
	f_packer			packer;
}						t_format;

// TODO in .c file
static const t_format	implemented_formats[FMT_SIZE] =
{
	[FMT_ELF64] = {elf64_identifier, elf64_packer},
	[FMT_ELF32] = {elf32_identifier, elf32_packer},
	[FMT_MACHO64] = {macho64_identifier, macho64_packer},
	[FMT_MACHO32] = {macho32_identifier, macho32_packer},
	[FMT_PE64] = {pe64_identifier, pe64_packer},
	[FMT_PE32] = {pe32_identifier, pe32_packer}
}

/*
** check file
*/

bool				check_eligibility(const Elf64_Ehdr *elf64_hdr);
const Elf64_Shdr	*get_entry_section(const __nonull Elf64_Ehdr *elf64_hdr, \
						__nonull Elf64_Off *entry_offset_in_sect);

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

# define WOODY_FATAL			"\033[31m[FATAL ERROR] \033[0m"
# define WOODY_WARN				"\033[33m[WARNING] \033[0m"

void			fatal(const char * const message);
void			warn(const char * const message);

#endif
