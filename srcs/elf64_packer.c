/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/13 01:40:05 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static const Elf64_Shdr	*get_entry_section(const __nonull Elf64_Ehdr *elf64_hdr, \
						Elf64_Off *entry_offset_in_sect)
{
	const Elf64_Addr	entry_addr = endian_8(elf64_hdr->e_entry);
	const Elf64_Off		shoff = endian_8(elf64_hdr->e_shoff);
	const Elf64_Half	shentsize = endian_2(elf64_hdr->e_shentsize);
	Elf64_Half			shnum = endian_2(elf64_hdr->e_shnum);
	char				(*sections)[shnum][shentsize] = NULL;

	if (shentsize < sizeof(Elf64_Shdr)
	|| (!(sections = safe(shoff, shentsize * shnum))))
	{
		dprintf(2, "invalid sections table\n");
		return (NULL);
	}

	while (shnum--)
	{
		const Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];
		const Elf64_Addr section_addr = endian_8(elf64_section_hdr->sh_addr);
		const Elf64_Xword section_size = endian_8(elf64_section_hdr->sh_size);

		if (section_addr <= entry_addr && entry_addr < section_addr + section_size)
		{
			*entry_offset_in_sect = entry_addr - section_addr;
			return (elf64_section_hdr);
		}
	}
	return (NULL);
}

void	say_hello(void);
void	end_hello(void);

bool	elf64_packer(__nonull void *clone, size_t original_filesize)
{
	Elf64_Ehdr		*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	void			*original = safe(0, original_filesize);

	if (original == NULL)
	{
		dprintf(2, "invalid original filesize");
		return (false);
	}
	memcpy(clone, original, original_filesize);

	// change clone's entry
	Elf64_Off			entry_offset_in_sect;
	const Elf64_Shdr	*entry_section = get_entry_section(elf64_hdr, &entry_offset_in_sect);
	if (entry_section == NULL)
	{
		dprintf(2, "entry point is not in a section");
		return (false);
	}
	const Elf64_Off		code_entry = endian_8(entry_section->sh_offset) + entry_offset_in_sect;
	memcpy(clone + code_entry, say_hello, end_hello - say_hello);

	return (true);
}
