/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_copy_to_clone.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 14:58:36 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/13 18:51:47 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	copy_until_end_of_text(size_t section_end_offset)
{
	void	*original = original_safe(0, section_end_offset);
	void	*clone    = clone_safe(0, section_end_offset);

	if (!original) return (errors(ERR_CORRUPT, "truncated file"));
	if (!clone) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	memcpy(clone, original, section_end_offset);
	return (true);
}

static bool	copy_after_payload(size_t end_of_text, size_t shift_amount, size_t original_file_size)
{
	const size_t	size_after_text = original_file_size - end_of_text;
	void		*original       = original_safe(end_of_text, size_after_text);
	void		*clone          = clone_safe(end_of_text + shift_amount, size_after_text);

	if (!original) return (errors(ERR_CORRUPT, "truncated file"));
	if (!clone) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	memcpy(clone, original, size_after_text);
	return true;
}

bool		copy_to_clone(size_t end_of_text, size_t shift_amount, \
			size_t original_file_size)
{
	if (!copy_until_end_of_text(end_of_text)
	|| !copy_after_payload(end_of_text, shift_amount, original_file_size))
		return (errors(ERR_THROW, "copy_to_clone"));
	return true;
}
