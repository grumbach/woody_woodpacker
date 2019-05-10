/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_viewer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 22:36:00 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/10 23:24:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"

static bool	print_ehdr(f_safe_accessor safe)
{
	Elf64_Ehdr *ehdr = safe(0, sizeof(*ehdr));

	if (ehdr == NULL) return errors(ERR_CORRUPT, "bad ehdr");

	printf("----------- Elf64 HEADER -----------\n"
	"unsigned char  e_ident[16]     = \"%.16s\"\n"
	"Elf64_Half     e_type          = %#.04hx\n"
	"Elf64_Half     e_machine       = %#.04hx\n"
	"Elf64_Word     e_version       = %#.08x\n"
	"Elf64_Addr     e_entry         = %#.016llx\n"
	"Elf64_Off      e_phoff         = %lu\n"
	"Elf64_Off      e_shoff         = %lu\n"
	"Elf64_Word     e_flags         = %#.08x\n"
	"Elf64_Half     e_ehsize        = %#.04hx\n"
	"Elf64_Half     e_phentsize     = %#.04hx\n"
	"Elf64_Half     e_phnum         = %hu\n"
	"Elf64_Half     e_shentsize     = %hu\n"
	"Elf64_Half     e_shnum         = %hu\n"
	"Elf64_Half     e_shstrndx      = %hu\n",
	ehdr->e_ident,
	ehdr->e_type,
	ehdr->e_machine,
	ehdr->e_version,
	ehdr->e_entry,
	ehdr->e_phoff,
	ehdr->e_shoff,
	ehdr->e_flags,
	ehdr->e_ehsize,
	ehdr->e_phentsize,
	ehdr->e_phnum,
	ehdr->e_shentsize,
	ehdr->e_shnum,
	ehdr->e_shstrndx);
	return true;
}

static bool	print_phdr(f_safe_accessor safe, size_t offset)
{
	Elf64_Phdr *phdr = safe(offset, sizeof(*phdr));

	if (phdr == NULL) return errors(ERR_CORRUPT, "bad phdr");

	printf("----------- program HEADER -----------\n"
	"Elf64_Word     p_type         = %#.08x\n"
	"Elf64_Word     p_flags        = %#.08x\n"
	"Elf64_Off      p_offset       = %lu\n"
	"Elf64_Addr     p_vaddr        = %#.016llx\n"
	"Elf64_Addr     p_paddr        = %#.016llx\n"
	"Elf64_Xword    p_filesz       = %llu\n"
	"Elf64_Xword    p_memsz        = %llu\n"
	"Elf64_Xword    p_align        = %llu\n",
	phdr->p_type,
	phdr->p_flags,
	phdr->p_offset,
	phdr->p_vaddr,
	phdr->p_paddr,
	phdr->p_filesz,
	phdr->p_memsz,
	phdr->p_align);

	return true;
}

static bool	print_shdr(f_safe_accessor safe, size_t offset)
{
	Elf64_Shdr *shdr = safe(offset, sizeof(*shdr));

	if (shdr == NULL) return errors(ERR_CORRUPT, "bad shdr");

	printf("----------- section HEADER -----------\n"
	"Elf64_Word     sh_name        = %#.08x\n"
	"Elf64_Word     sh_type        = %#.08x\n"
	"Elf64_Xword    sh_flags       = %llu\n"
	"Elf64_Addr     sh_addr        = %#.016llx\n"
	"Elf64_Off      sh_offset      = %lu\n"
	"Elf64_Xword    sh_size        = %llu\n"
	"Elf64_Word     sh_link        = %#.08x\n"
	"Elf64_Word     sh_info        = %#.08x\n"
	"Elf64_Xword    sh_addralign   = %llu\n"
	"Elf64_Xword    sh_entsize     = %llu\n",
	shdr->sh_name,
	shdr->sh_type,
	shdr->sh_flags,
	shdr->sh_addr,
	shdr->sh_offset,
	shdr->sh_size,
	shdr->sh_link,
	shdr->sh_info,
	shdr->sh_addralign,
	shdr->sh_entsize);

	return true;
}

bool	elf64_viewer(f_safe_accessor safe)
{
	print_ehdr(safe);
	iterate_phdr(safe, print_phdr);
	iterate_shdr(safe, print_shdr);
}
