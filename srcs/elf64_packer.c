/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/13 07:57:10 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include "elf64_private.h"

static bool	copy_everything_before_entry_segment(const __nonull t_entry *entry, \
				void *clone)
{
	size_t		size_before_entry = entry->seg_hdr->p_offset;
	void		*data_before_entry = safe(0, size_before_entry);

	if (data_before_entry == NULL)
		return (errors(ERR_CORRUPT, "failed to read before entry segment"));

	memcpy(clone, data_before_entry, size_before_entry);

	return (true);
}

// TODO actually add the payload !
static bool	add_payloaded_entry_segment(const __nonull t_entry *entry, \
				void *clone_entry_segment)
{
	size_t		entry_segment_size = entry->seg_hdr->p_filesz;
	void		*entry_segment_data = safe(entry->seg_hdr->p_offset, entry_segment_size);

	if (entry_segment_data == NULL)
		return (errors(ERR_CORRUPT, "failed to read entry segment"));

	memcpy(clone_entry_segment, entry_segment_data, entry_segment_size);

	return (true);
}

// TODO move all references
static bool	copy_everything_after_entry_segment(const __nonull t_entry *entry, \
				void *clone_after_entry_segment, size_t original_filesize)
{
	size_t		offset_after_entry = entry->seg_hdr->p_offset + entry->seg_hdr->p_filesz;
	size_t		size_after_entry = original_filesize - offset_after_entry;
	void		*data_after_entry = safe(offset_after_entry, size_after_entry);

	if (data_after_entry == NULL)
		return (errors(ERR_CORRUPT, "failed to read after entry segment"));

	memcpy(clone_after_entry_segment, data_after_entry, size_after_entry);

	return (true);
}

bool	elf64_packer(__nonull void *clone, size_t original_filesize)
{
	Elf64_Ehdr		*original_hdr = safe(0, sizeof(Elf64_Ehdr));
	t_entry			entry;

	if (original_hdr == NULL)
		return (errors(ERR_CORRUPT, "invalid elf64 header"));

	if (!find_entry(&entry, original_hdr)
	|| !copy_everything_before_entry_segment(&entry, clone)
	|| !add_payloaded_entry_segment(&entry, clone + entry.seg_hdr->p_offset)
	|| !copy_everything_after_entry_segment(&entry, clone + entry.seg_hdr->p_offset + entry.seg_hdr->p_filesz, original_filesize))
		return (errors(ERR_THROW, __func__));

	return (true);
}
