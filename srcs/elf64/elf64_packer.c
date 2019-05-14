/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/14 19:11:57 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	change_entry(const struct entry *original_entry)
{
	Elf64_Ehdr	*clone_hdr = clone_safe(0, sizeof(Elf64_Ehdr));

	if (!clone_hdr) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	const Elf64_Xword	sh_offset         = endian_8(original_entry->safe_shdr->sh_offset);
	const size_t		offset_in_section = original_entry->offset_in_section;
	const size_t		entry_off         = sh_offset + offset_in_section;
	const size_t		payload_offset    = original_entry->end_of_last_section;
	const Elf64_Xword	payload_distance  = payload_offset - entry_off;

	Elf64_Addr		e_entry = endian_8(clone_hdr->e_entry);

	e_entry += payload_distance;
	clone_hdr->e_entry = endian_8(e_entry);

	return true;
}

static bool	adjust_sizes(size_t shift_amount)
{
	struct entry	clone_entry;

	if (!find_entry(&clone_entry, clone_safe))
		return errors(ERR_THROW, "adjust_sizes");

	Elf64_Xword	p_filesz = endian_8(clone_entry.safe_phdr->p_filesz);
	Elf64_Xword	p_memsz  = endian_8(clone_entry.safe_phdr->p_memsz);
	Elf64_Xword	sh_size  = endian_8(clone_entry.safe_shdr->sh_size);

	p_filesz += shift_amount;
	p_memsz  += shift_amount;
	sh_size  += shift_amount;

	clone_entry.safe_phdr->p_filesz = endian_8(p_filesz);
	clone_entry.safe_phdr->p_memsz  = endian_8(p_memsz);
	clone_entry.safe_shdr->sh_size  = endian_8(sh_size);

	return true;
}

bool		elf64_packer(size_t original_file_size)
{
	const size_t	payload_size = end_payload - begin_payload;
	const size_t	shift_amount = ALIGN(payload_size, WOODY_ALIGNMENT); // TODO change this!
	struct entry	original_entry;

	if (!find_entry(&original_entry, original_safe)
	|| !copy_to_clone(original_entry.end_of_last_section, shift_amount, original_file_size)
	|| !adjust_references(shift_amount, &original_entry)
	|| !adjust_sizes(shift_amount)
	|| !setup_payload(&original_entry)
	|| !change_entry(&original_entry))
		return errors(ERR_THROW, "elf64_packer");

	elf64_viewer(clone_safe);

	return true;
}
