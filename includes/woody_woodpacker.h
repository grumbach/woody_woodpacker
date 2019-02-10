/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/10 18:32:21 by agrumbac         ###   ########.fr       */
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

/*
** check file
*/

bool				check_eligibility(const Elf64_Ehdr *elf64_hdr);
const Elf64_Shdr	*get_entry_section(const __nonull Elf64_Ehdr *elf64_hdr);
/*
** encryption
*/

void			encrypt(char *data, size_t size, \
						uint num_rounds, uint32_t const key[4]);
void			decrypt(char *data, size_t size, \
						uint num_rounds, uint32_t const key[4]);

/*
** safe file accessing
*/

__warn_unused_result
void			*safe(const Elf64_Off offset, const size_t size);
void			read_file(const char *filename);
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
