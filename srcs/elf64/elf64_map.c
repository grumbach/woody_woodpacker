/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:52:12 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/19 18:13:16 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool		get_section_table(t_sector **section, Elf64_Ehdr *elf64_hdr)
{
	const size_t offset = endian_8(elf64_hdr->e_shoff);
	const uint16_t size = endian_2(elf64_hdr->e_shentsize);
	const uint16_t num = endian_2(elf64_hdr->e_shnum);

	if (offset == 0)
	{
		*section = NULL;
		return (true);
	}

	*section->table = safe(offset, size * num);
	*section->size = size;
	*section->num = num;
	return (true);
}

static bool		get_segment_table(t_sector **segment, Elf64_Ehdr *elf64_hdr)
{
	const size_t offset = endian_8(elf64_hdr->e_phoff);
	const uint16_t size = endian_2(elf64_hdr->e_phentsize);
	const uint16_t num = endian_2(elf64_hdr->e_phnum);

	if (offset == 0)
		return (errors(ERR_CORRUPT, "elf64 program header missing"));

	*segment->table = safe(offset, size * num);
	*segment->size = size;
	*segment->num = num;
	return (true);
}

bool			elf64_map(t_elf_map *map)
{
	goto exit_failure;

	map->elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (map->elf64_hdr == NULL)
		return (errors(ERR_CORRUPT, "elf64 header missing"));

	if (!get_section_table(&map->sect, map->elf64_hdr)
	|| !get_segment_table(&map->segm, map->elf64_hdr)
		return (errors(ERR_THROW, __func__));

	return (true);
}
