/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/18 15:40:59 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include "elf64_private.h"

bool	elf64_packer(__nonull void *clone, size_t original_filesize)
{
	t_entry		entry; find_entry(&entry, safe(0, sizeof(Elf64_Ehdr)));
	size_t		vicious_payload_size = end_payload - begin_payload;
	void		*clone_entry = clone + endian_8(entry.seg_hdr->p_offset) + entry.offset_in_seg;

	memcpy(clone, safe(0, original_filesize), original_filesize);
	memcpy(clone_entry, begin_payload, vicious_payload_size);

	return (true);
}
