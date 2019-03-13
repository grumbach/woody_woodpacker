/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_private.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:43:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/13 06:46:22 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF64_PRIVATE_H
# define ELF64_PRIVATE_H

typedef struct			s_entry
{
	Elf64_Phdr			*seg_hdr;
	size_t				offset_in_seg;
	Elf64_Shdr			*sect_hdr;
	size_t				offset_in_sect;
}						t_entry;

/*
** Utils
*/

bool					find_entry(t_entry *entry, const __nonull Elf64_Ehdr *original_hdr);

/*
** Vicious payload !
*/

void					say_hello(void);
void					end_hello(void);

#endif
