/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_find_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 23:43:29 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/11 00:00:01 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static Elf64_Shdr	*entry_shdr()
{
	// Elf64_Shdr *elf64_section_hdr =
	// const Elf64_Addr section_addr = endian_8(elf64_section_hdr->sh_addr);
	// const Elf64_Xword section_size = endian_8(elf64_section_hdr->sh_size);
	//
	// if (section_addr <= entry_addr && entry_addr < section_addr + section_size)
	// {
	// 	entry->sect_hdr = elf64_section_hdr;
	// 	entry->offset_in_sect = entry_addr - section_addr;
	// 	return ();
	// }
}

static Elf64_Phdr	*entry_shdr()
{
	// Elf64_Phdr *elf64_seg_hdr =
	// const Elf64_Addr seg_addr = endian_8(elf64_seg_hdr->p_vaddr);
	// const Elf64_Xword seg_size = endian_8(elf64_seg_hdr->p_memsz);
	//
	// if (seg_addr <= entry_addr && entry_addr < seg_addr + seg_size)
	// {
	// 	entry->seg_hdr = elf64_seg_hdr;
	// 	entry->offset_in_seg = entry_addr - seg_addr;
	// 	return ();
	// }
}

bool	find_entry(struct entry *original_entry, f_safe_accessor safe)
{
	iterate_phdr()
	iterate_shdr()
}
