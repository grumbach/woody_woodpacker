/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/13 06:59:47 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include "elf64_private.h"

bool	elf64_packer(__nonull void *clone, __unused size_t original_filesize)
{
	Elf64_Ehdr		*original_hdr = safe(0, sizeof(Elf64_Ehdr));
	Elf64_Ehdr		*clone_hdr = clone;
	t_entry			entry;

	if (original_hdr == NULL)
		errors(ERR_CORRUPT, "invalid elf64 header");

	if (find_entry(&entry, original_hdr) == false)
		return (errors(ERR_THROW, __func__));

	printf("%p, %lu, %p, %lu\n", entry.seg_hdr, entry.offset_in_seg, entry.sect_hdr, entry.offset_in_sect);

	// encrypt_entry_segment();
	// add_payload();

	return (true);
}
