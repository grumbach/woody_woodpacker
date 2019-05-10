/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_iterators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 08:11:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/10 08:35:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	iterate_phdr(f_safe_accessor safe, f_iter_callback callback)
{
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (elf64_hdr == NULL)
		return (errors(ERR_CORRUPT, "no elf64 hdr"));

	const Elf64_Off		phoff = endian_8(elf64_hdr->e_phoff);
	const Elf64_Half	phentsize = endian_2(elf64_hdr->e_phentsize);
	Elf64_Half		phnum = endian_2(elf64_hdr->e_phnum);
	char			(*segments)[phnum][phentsize] = NULL;

	if (phentsize < sizeof(Elf64_Phdr)
	|| (!(segments = safe(phoff, phentsize * phnum))))
		return (errors(ERR_CORRUPT, "invalid segments table"));

	while (phnum--)
	{
		Elf64_Phdr *elf64_seg_hdr = (Elf64_Phdr *)(*segments)[phnum];

		// now what?
	}
	return (true);
}

static bool	iterate_shdr(f_safe_accessor safe, f_iter_callback callback)
{
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (elf64_hdr == NULL)
		return (errors(ERR_CORRUPT, "no elf64 hdr"));

	const Elf64_Off		shoff = endian_8(elf64_hdr->e_shoff);
	const Elf64_Half	shentsize = endian_2(elf64_hdr->e_shentsize);
	Elf64_Half		shnum = endian_2(elf64_hdr->e_shnum);
	char			(*sections)[shnum][shentsize] = NULL;

	if (shentsize < sizeof(Elf64_Shdr)
	|| (!(sections = safe(shoff, shentsize * shnum))))
		return (errors(ERR_CORRUPT, "invalid sections table"));

	while (shnum--)
	{
		Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];

		// now what?
	}
	return (true);
}
