/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_copy_to_clone.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 14:58:36 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/03 22:01:43 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	copy_until_end_of_last_sect(size_t end_of_last_section)
{
	void	*original = original_safe(0, end_of_last_section);
	void	*clone    = clone_safe(0, end_of_last_section);

	if (!original) return (errors(ERR_CORRUPT, "truncated file"));
	if (!clone) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	memcpy(clone, original, end_of_last_section);
	return (true);
}

static bool	copy_after_payload(size_t end_last_sect, \
			size_t shift_amount, size_t original_size)
{
	const size_t	size_after_last_sect = original_size - end_last_sect;
	void		*original = original_safe(end_last_sect, size_after_last_sect);
	void		*clone    = clone_safe(end_last_sect + shift_amount, size_after_last_sect);

	if (!original) return (errors(ERR_CORRUPT, "truncated file"));
	if (!clone) return (errors(ERR_CORRUPT, "wildly unreasonable"));

	memcpy(clone, original, size_after_last_sect);
	return true;
}

bool		copy_to_clone(size_t end_last_sect, size_t shift_amount, \
			size_t original_size)
{
	if (!copy_until_end_of_last_sect(end_last_sect)
	|| !copy_after_payload(end_last_sect, shift_amount, original_size))
		return (errors(ERR_THROW, "copy_to_clone"));
	return true;
}
