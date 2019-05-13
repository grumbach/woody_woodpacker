/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_adjust_references.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 14:56:28 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/13 15:36:33 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static struct
{
	size_t	shift_amount;
	size_t	end_of_text;
}		closure;

static bool	shift_phdr_position()
{

}

static bool	shift_shdr_position()
{

}

bool		adjust_references(size_t shift_amount, size_t end_of_text)
{
	closure.shift_amount = shift_amount;
	closure.end_of_text = end_of_text;

	adjust_phdr_table_offset(shift_amount, end_of_text);
	adjust_shdr_table_offset(shift_amount, end_of_text);

	foreach_phdr(original_safe, shift_phdr_position);
	foreach_shdr(original_safe, shift_shdr_position);
}
