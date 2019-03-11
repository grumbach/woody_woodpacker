/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_identifier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:01 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/11 15:52:28 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

bool	elf64_identifier(void)
{
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(*elf64_hdr));

	if (elf64_hdr == NULL                                // no header at all!
	|| memcmp(elf64_hdr->e_ident, ELFMAG, SELFMAG) != 0  // wrong Magic
	|| elf64_hdr->e_ident[EI_CLASS] != ELFCLASS64        // not 64bit
	|| elf64_hdr->e_type != ET_EXEC                      // not executable file
	|| elf64_hdr->e_entry == 0)                          // no entry point
		return (false);

	// set endian for the future
	endian_big_mode(elf64_hdr->e_ident[EI_DATA] == ELFDATA2MSB);

	return (true);
}
