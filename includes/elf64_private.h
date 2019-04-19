/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_private.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:43:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/04/19 17:14:28 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF64_PRIVATE_H
# define ELF64_PRIVATE_H

# include <linux/elf.h>
# include "woody_woodpacker.h"

/*
** elf_map:
**   - every single pointer in an elf_map is safe
**   - every raw value is in host byte order
**   - everything else is in file byte order
** sector:
**   - segment table
**   - section table
*/

typedef struct		s_entry
{
	void		*sector;
	size_t		offset;
}			t_entry;

typedef struct		s_sector
{
	void		*table;
	size_t		size;
	size_t		num;
	t_entry		entry;
}			t_sector;

typedef struct		s_elf_map
{
	Elf64_Ehdr	*elf64_hdr;
	t_sector	*segm;
	t_sector	*sect;
}			t_elf_map;

/*
** Utils
*/

bool	find_entry(t_entry *entry, const __nonull Elf64_Ehdr *original_hdr);
bool	elf64_map(t_elf_map *map);

/*
** Vicious payload !
*/

void	begin_payload(void);
void	end_payload(void);

#endif
