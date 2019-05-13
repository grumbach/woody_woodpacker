/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_adjust_sizes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 16:20:29 by jfortin           #+#    #+#             */
/*   Updated: 2019/05/13 17:06:54 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	adjust_sizes(shift_amount)
{
	struct entry	clone_entry;

	if (!find_entry(&clone_entry, clone_safe))
		return (errors(ERR_THROW, "adjust_sizes"));
	clone_entry.safe_shdr += shift_amount;
	clone_entry.safe_phdr += shift_amount;
	return true;
}