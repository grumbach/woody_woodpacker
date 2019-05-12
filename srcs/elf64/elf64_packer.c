/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 07:31:03 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	change_entry(const struct entry *original_entry)
{
	Elf64_Ehdr	*clone_hdr = clone_safe(0, sizeof(Elf64_Ehdr));

	if (!clone_hdr) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	const Elf64_Xword sh_size = endian_8(original_entry->safe_shdr->sh_size);
	const Elf64_Xword payload_distance = sh_size - original_entry->offset_in_section;

	clone_hdr->e_entry += payload_distance;

	return (true);
}

static bool	copy_until_end_of_text(size_t section_end_offset)
{
	void	*original = original_safe(0, section_end_offset);
	void	*clone    = clone_safe(0, section_end_offset);

	if (!original) return (errors(ERR_CORRUPT, "wildly unreasonable"));
	if (!clone) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	memcpy(clone, original, section_end_offset);
	return (true);
}

bool		elf64_packer(void)
{
	struct entry	original_entry;

	// protect all calls below
	find_entry(&original_entry, original_safe);
	copy_until_end_of_text(original_entry.section_end_offset);
	setup_payload(&original_entry);
	// copy_after_payload();
	change_entry(&original_entry);

	return (true);
}
