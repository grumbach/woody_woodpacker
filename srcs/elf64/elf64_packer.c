/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/13 16:22:50 by agrumbac         ###   ########.fr       */
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

static bool	adjust_sizes(shift_amount)
{
	grow_text_section(shift_amount);
	grow_pt_load(shift_amount);
}

bool		elf64_packer(size_t original_file_size)
{
	const size_t	payload_size = end_payload - begin_payload;
	const size_t	shift_amount = ALIGN(payload_size, WOODY_ALIGNMENT);
	struct entry	original_entry;

	// protect all calls below
	find_entry(&original_entry, original_safe);
	copy_to_clone(original_entry.section_end_offset, shift_amount, original_file_size);
	adjust_references(shift_amount, original_entry.section_end_offset);
	adjust_sizes(shift_amount);
	setup_payload(&original_entry);
	change_entry(&original_entry);

	return (true);
}
