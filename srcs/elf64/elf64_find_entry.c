/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_find_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 23:43:29 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/14 19:26:47 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static struct entry	*stored_entry;
static Elf64_Ehdr	*safe_elf64_hdr;
static Elf64_Addr	e_entry;

static bool	find_entry_shdr(f_safe_accessor safe, const size_t offset)
{
	Elf64_Shdr		*elf64_sect_hdr = safe(offset, sizeof(Elf64_Shdr));

	if (!elf64_sect_hdr) return (errors(ERR_CORRUPT, "bad shdr offset"));

	const Elf64_Addr	sect_addr = endian_8(elf64_sect_hdr->sh_addr);
	const Elf64_Xword	sect_size = endian_8(elf64_sect_hdr->sh_size);

	if (sect_addr <= e_entry && e_entry < sect_addr + sect_size)
		stored_entry->safe_shdr = elf64_sect_hdr;
	if (sect_size + offset > stored_entry->end_of_last_section)
		stored_entry->end_of_last_section = sect_size + offset;
	return true;
}

static bool	find_entry_phdr(f_safe_accessor safe, const size_t offset)
{
	Elf64_Phdr		*elf64_seg_hdr = safe(offset, sizeof(Elf64_Phdr));

	if (!elf64_seg_hdr) return (errors(ERR_CORRUPT, "bad phdr offset"));

	const Elf64_Addr	seg_addr = endian_8(elf64_seg_hdr->p_vaddr);
	const Elf64_Xword	seg_size = endian_8(elf64_seg_hdr->p_memsz);

	if (seg_addr <= e_entry && e_entry < seg_addr + seg_size)
		stored_entry->safe_phdr = elf64_seg_hdr;
	return true;
}

bool		find_entry(struct entry *original_entry, f_safe_accessor safe)
{
	safe_elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (!safe_elf64_hdr) return (errors(ERR_CORRUPT, "missing elf64_hdr"));
	e_entry = endian_8(safe_elf64_hdr->e_entry);

	bzero(original_entry, sizeof(*original_entry));
	stored_entry = original_entry;

	if (!foreach_phdr(safe, find_entry_phdr))
		return (errors(ERR_THROW, "find_entry"));
	if (!foreach_shdr(safe, find_entry_shdr))
		return (errors(ERR_THROW, "find_entry"));

	if (!original_entry->safe_shdr || !original_entry->safe_phdr)
		return (errors(ERR_CORRUPT, "missing entry section or segment"));

	const Elf64_Addr sh_addr  = endian_8(original_entry->safe_shdr->sh_addr);

	original_entry->end_of_last_section = sh_offset + sh_size;
	original_entry->offset_in_section = e_entry - sh_addr;

	return true;
}
