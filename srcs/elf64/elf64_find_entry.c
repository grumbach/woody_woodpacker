/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_find_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 23:43:29 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/03 21:24:46 by agrumbac         ###   ########.fr       */
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

	const Elf64_Addr	sh_addr = endian_8(elf64_sect_hdr->sh_addr);
	const Elf64_Xword	sh_size = endian_8(elf64_sect_hdr->sh_size);

	if (sh_addr <= e_entry && e_entry < sh_addr + sh_size)
		stored_entry->safe_shdr = elf64_sect_hdr;

	const Elf64_Off		p_offset  = endian_8(stored_entry->safe_phdr->p_offset);
	const Elf64_Off		sh_offset = endian_8(elf64_sect_hdr->sh_offset);
	const Elf64_Xword	p_filesz  = endian_8(stored_entry->safe_phdr->p_filesz);

	const size_t		end_of_ptload = p_offset + p_filesz;
	const size_t		end_of_sect   = sh_offset + sh_size;

	if (end_of_sect <= end_of_ptload
	&& (end_of_sect > stored_entry->end_of_last_section))
	{
		stored_entry->safe_last_section_shdr = elf64_sect_hdr;
		stored_entry->end_of_last_section = end_of_sect;
	}

	return true;
}

static bool	find_entry_phdr(f_safe_accessor safe, const size_t offset)
{
	Elf64_Phdr		*elf64_seg_hdr = safe(offset, sizeof(Elf64_Phdr));

	if (!elf64_seg_hdr) return (errors(ERR_CORRUPT, "bad phdr offset"));

	const Elf64_Addr	p_vaddr = endian_8(elf64_seg_hdr->p_vaddr);
	const Elf64_Xword	p_memsz = endian_8(elf64_seg_hdr->p_memsz);

	if (p_vaddr <= e_entry && e_entry < p_vaddr + p_memsz)
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
	if (!original_entry->safe_phdr)
		return (errors(ERR_CORRUPT, "missing entry segment"));

	if (!foreach_shdr(safe, find_entry_shdr))
		return (errors(ERR_THROW, "find_entry"));
	if (!original_entry->safe_shdr)
		return (errors(ERR_CORRUPT, "missing entry section"));

	const Elf64_Addr sh_addr  = endian_8(original_entry->safe_shdr->sh_addr);

	original_entry->offset_in_section = e_entry - sh_addr;

	if (original_entry->end_of_last_section == 0)
		errors(ERR_CORRUPT, "no section in entry segment");

	return true;
}
