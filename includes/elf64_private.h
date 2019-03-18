/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_private.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:43:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/15 13:54:55 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF64_PRIVATE_H
# define ELF64_PRIVATE_H

typedef struct			s_clone
{
	Elf64_Ehdr			*elf64_hdr;
	void				*section_hdr_table;
	void				*program_hdr_table;
}						t_clone;

typedef struct			s_entry
{
	Elf64_Phdr			*seg_hdr;
	size_t				offset_in_seg;
	Elf64_Shdr			*sect_hdr;
	size_t				offset_in_sect;
}						t_entry;

typedef struct			s_payload
{
	void				*code;
	size_t				size;
}						t_payload;

/*
** Utils
*/

bool					find_entry(t_entry *entry, const __nonull Elf64_Ehdr *original_hdr);

/*
** Vicious payload !
*/

void					begin_payload(void);
void					end_payload(void);

#endif
