/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_find_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 05:46:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/19 18:07:34 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "elf64_private.h"
//
// static bool			get_entry_section(t_entry *entry, \
// 						const __nonull Elf64_Ehdr *elf64_hdr, \
// 						const Elf64_Addr entry_addr)
// {
// 	const Elf64_Off		shoff = endian_8(elf64_hdr->e_shoff);
// 	const Elf64_Half	shentsize = endian_2(elf64_hdr->e_shentsize);
// 	Elf64_Half			shnum = endian_2(elf64_hdr->e_shnum);
// 	char				(*sections)[shnum][shentsize] = NULL;
//
// 	if (shentsize < sizeof(Elf64_Shdr)
// 	|| (!(sections = safe(shoff, shentsize * shnum))))
// 		return (errors(ERR_CORRUPT, "invalid sections table"));
//
// 	while (shnum--)
// 	{
// 		Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];
// 		const Elf64_Addr section_addr = endian_8(elf64_section_hdr->sh_addr);
// 		const Elf64_Xword section_size = endian_8(elf64_section_hdr->sh_size);
//
// 		if (section_addr <= entry_addr && entry_addr < section_addr + section_size)
// 		{
// 			entry->sect_hdr = elf64_section_hdr;
// 			entry->offset_in_sect = entry_addr - section_addr;
// 			return (true);
// 		}
// 	}
// 	entry->sect_hdr = NULL;
// 	entry->offset_in_sect = 0;
// 	return (true);
// }
//
// static bool			get_entry_segment(t_entry *entry, \
// 						const __nonull Elf64_Ehdr *elf64_hdr, \
// 						const Elf64_Addr entry_addr)
// {
// 	const Elf64_Off		phoff = endian_8(elf64_hdr->e_phoff);
// 	const Elf64_Half	phentsize = endian_2(elf64_hdr->e_phentsize);
// 	Elf64_Half			phnum = endian_2(elf64_hdr->e_phnum);
// 	char				(*segments)[phnum][phentsize] = NULL;
//
// 	if (phentsize < sizeof(Elf64_Phdr)
// 	|| (!(segments = safe(phoff, phentsize * phnum))))
// 		return (errors(ERR_CORRUPT, "invalid segments table"));
//
// 	while (phnum--)
// 	{
// 		Elf64_Phdr *elf64_seg_hdr = (Elf64_Phdr *)(*segments)[phnum];
// 		const Elf64_Addr seg_addr = endian_8(elf64_seg_hdr->p_vaddr);
// 		const Elf64_Xword seg_size = endian_8(elf64_seg_hdr->p_memsz);
//
// 		if (seg_addr <= entry_addr && entry_addr < seg_addr + seg_size)
// 		{
// 			entry->seg_hdr = elf64_seg_hdr;
// 			entry->offset_in_seg = entry_addr - seg_addr;
// 			return (true);
// 		}
// 	}
// 	entry->seg_hdr = NULL;
// 	entry->offset_in_seg = 0;
// 	return (true);
// }
//
// bool				find_entry(t_entry *entry, const __nonull Elf64_Ehdr *original_hdr)
// {
// 	const Elf64_Addr	entry_addr = endian_8(original_hdr->e_entry);
//
// 	if (get_entry_section(entry, original_hdr, entry_addr) == false
// 	|| get_entry_segment(entry, original_hdr, entry_addr) == false)
// 		return (errors(ERR_THROW, __func__));
//
// 	return (true);
// }
