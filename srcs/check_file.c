/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 15:42:10 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/10 20:26:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

bool		check_eligibility(const Elf64_Ehdr *elf64_hdr)
{
	if (elf64_hdr == NULL                                // no header at all!
	|| memcmp(elf64_hdr->e_ident, ELFMAG, SELFMAG) != 0  // wrong Magic
	|| elf64_hdr->e_ident[EI_CLASS] != ELFCLASS64        // not 64bit
	|| elf64_hdr->e_type != ET_EXEC                      // not executable file
	|| elf64_hdr->e_entry == 0)                          // no entry point
	{
		dprintf(2, WOODY_WARN "invalid file format\n");
		return (false);
	}
	return (true);
}

const Elf64_Shdr	*get_entry_section(const __nonull Elf64_Ehdr *elf64_hdr)
{
	const Elf64_Addr    entry_addr = endian_8(elf64_hdr->e_entry);
	const Elf64_Half	shentsize = endian_2(elf64_hdr->e_shentsize);
	Elf64_Half			shnum = endian_2(elf64_hdr->e_shnum);
	char				(*sections)[shnum][shentsize] = NULL;

	if (shentsize < sizeof(Elf64_Shdr)
	|| (!(sections = safe(elf64_hdr->e_shoff, shentsize * shnum))))
	{
		dprintf(2, WOODY_WARN "invalid sections table\n");
		return (NULL);
	}

	while (shnum--)
	{
		const Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];
		const Elf64_Addr section_addr = endian_8(elf64_section_hdr->sh_addr);
		const Elf64_Xword section_size = endian_8(elf64_section_hdr->sh_size);

		if (endian_4(elf64_section_hdr->sh_type) == SHT_PROGBITS)
		{
			printf("Executable sect at index %hu (%p) [", shnum, elf64_section_hdr);
			if (endian_8(elf64_section_hdr->sh_flags) & SHF_ALLOC)
				printf("-A");
			if (endian_8(elf64_section_hdr->sh_flags) & SHF_EXECINSTR)
				printf("-E");
			if (endian_8(elf64_section_hdr->sh_flags) & SHF_WRITE)
				printf("-W");
			printf("]\n");
		}

		if (section_addr <= entry_addr && entry_addr < section_addr + section_size)
		{
			printf("Found entry!\n");
			return (elf64_section_hdr);
		}
	}
	return (NULL);
}
