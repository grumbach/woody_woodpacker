/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_adjust_references.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 14:56:28 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/13 18:27:54 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static struct
{
	size_t	shift_amount;
	size_t	end_of_text;
	size_t	entry_sh_addr;
	size_t	entry_p_vaddr;
	size_t	entry_p_paddr;
}		closure;

static bool	shift_phdr_position(f_safe_accessor safe, const size_t offset)
{
	Elf64_Phdr	*phdr = safe(offset, sizeof(Elf64_Phdr));
	if (phdr == NULL) return errors(ERR_CORRUPT, "bad phdr offset");

	Elf64_Off	p_offset = endian_8(phdr->p_offset);
	Elf64_Addr	p_vaddr = endian_8(phdr->p_vaddr);
	Elf64_Addr	p_paddr = endian_8(phdr->p_paddr);

	if (p_offset >= closure.end_of_text)
	{
		p_offset      += closure.shift_amount;
		phdr->p_offset = endian_8(p_offset);
	}
	if (p_vaddr > closure.entry_p_vaddr)
	{
		p_vaddr      += closure.shift_amount;
		phdr->p_vaddr = endian_8(p_vaddr);
	}
	if (p_paddr > closure.entry_p_paddr)
	{
		p_paddr      += closure.shift_amount;
		phdr->p_paddr = endian_8(p_paddr);
	}
	return true;
}

static bool	shift_shdr_position(f_safe_accessor safe, const size_t offset)
{
	Elf64_Shdr	*shdr = safe(offset, sizeof(Elf64_Shdr));
	if (shdr == NULL) return errors(ERR_CORRUPT, "bad shdr offset");

	Elf64_Off	sh_offset = endian_8(shdr->sh_offset);
	Elf64_Addr	sh_addr = endian_8(shdr->sh_addr);

	if (sh_offset >= closure.end_of_text)
	{
		sh_offset      += closure.shift_amount;
		shdr->sh_offset = endian_8(sh_offset);
	}
	if (sh_addr > closure.entry_sh_addr)
	{
		sh_addr      += closure.shift_amount;
		shdr->sh_addr = endian_8(sh_addr);
	}
	return true;
}

static void	adjust_phdr_table_offset(Elf64_Ehdr *safe_elf_hdr, \
			size_t shift_amount, size_t end_of_text)
{
	Elf64_Off	e_phoff = endian_8(safe_elf_hdr->e_phoff);

	if (e_phoff < end_of_text)
		return ;

	e_phoff              += shift_amount;
	safe_elf_hdr->e_phoff = endian_8(e_phoff);
}

static void	adjust_shdr_table_offset(Elf64_Ehdr *safe_elf_hdr, \
			size_t shift_amount, size_t end_of_text)
{
	Elf64_Off	e_shoff = endian_8(safe_elf_hdr->e_shoff);

	if (e_shoff < end_of_text)
		return ;

	e_shoff              += shift_amount;
	safe_elf_hdr->e_shoff = endian_8(e_shoff);
}

bool		adjust_references(size_t shift_amount, const struct entry *original_entry)
{
	closure.shift_amount  = shift_amount;
	closure.end_of_text   = original_entry->section_end_offset;
	closure.entry_sh_addr = endian_8(original_entry->safe_shdr->sh_addr);
	closure.entry_p_vaddr = endian_8(original_entry->safe_phdr->p_vaddr);
	closure.entry_p_paddr = endian_8(original_entry->safe_phdr->p_paddr);

	Elf64_Ehdr	*elf_hdr = clone_safe(0, sizeof(Elf64_Ehdr));
	if (elf_hdr == NULL) return errors(ERR_CORRUPT, "wildly unreasonable");

	adjust_phdr_table_offset(elf_hdr, shift_amount, closure.end_of_text);
	adjust_shdr_table_offset(elf_hdr, shift_amount, closure.end_of_text);

	if (!foreach_phdr(clone_safe, shift_phdr_position))
		return errors(ERR_THROW, "adjust_references");
	if (!foreach_shdr(clone_safe, shift_shdr_position))
		return errors(ERR_THROW, "adjust_references");

	return true;
}
