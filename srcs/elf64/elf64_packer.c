/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/11 00:01:17 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

bool	elf64_packer(void *clone, size_t clone_filesize)
{
	struct entry	original_entry;

	elf64_viewer(original_safe);

	find_entry(&original_entry, original_safe);
	copy_until_end_of_text(clone, original_entry.section_end_offset);
	// setup_payload(clone, &original_entry);
	// copy_after_end_of_text(clone); //clone + payload_size
	return (true);
}
