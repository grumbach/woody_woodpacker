/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_copy_after_payload.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 02:52:51 by jfortin           #+#    #+#             */
/*   Updated: 2019/05/12 03:52:53 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

bool	copy_after_payload(struct entry clone_entry, size_t payload_size)
{
	increase_size_ptload(clone_entry.safe_phdr);
	increase_size_text_section(clone_entry.safe_shdr);

	cpy_ptable = copy_program_table(original_ptable);
	cpy_stable = copy_section_table(original_stable);
	sort_program_table(cpy_ptable);
	sort_section_table(cpy_stable);

	memcpy(all_after_payload);

	update_segments_offset();
	update_sections_offset();
}

clone_safe(offset, size);