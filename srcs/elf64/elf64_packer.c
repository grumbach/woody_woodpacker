/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/04/19 17:14:43 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

bool	elf64_packer(__nonull void *clone, size_t original_filesize)
{
	t_sector	segm;
	t_sector	sect;
	t_elf_map	original_map = {.segm = &segm, .sect = &sect};

	elf64_map(&original_map);

	return (true);
}
