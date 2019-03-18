/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_packer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/18 09:04:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include "elf64_private.h"

/*
** ---------------------------------- copy -------------------------------------
*/

static bool	copy_everything_before_entry_segment(const __nonull t_entry *entry, \
				void *clone)
{
	size_t		size_before_entry = endian_8(entry->seg_hdr->p_offset);
	void		*data_before_entry = safe(0, size_before_entry);

	if (data_before_entry == NULL)
		return (errors(ERR_CORRUPT, "failed to read before entry segment"));

	memcpy(clone, data_before_entry, size_before_entry);

	return (true);
}

static bool	add_payloaded_entry_segment(const __nonull t_entry *entry, \
				void *clone_entry_segment)
{
	// copy original entry segment
	size_t		entry_segment_size = endian_8(entry->seg_hdr->p_filesz);
	void		*entry_segment_data = safe(endian_8(entry->seg_hdr->p_offset), entry_segment_size);

	if (entry_segment_data == NULL)
		return (errors(ERR_CORRUPT, "failed to read entry segment"));
	memcpy(clone_entry_segment, entry_segment_data, entry_segment_size);

	// copy payload after it
	void		*clone_after_entry = clone_entry_segment + entry_segment_size;
	size_t		vicious_payload_size = ((end_payload - begin_payload) + 8 - ((end_payload - begin_payload) % 8));

	memcpy(clone_after_entry, begin_payload, vicious_payload_size);

	return (true);
}

static bool	copy_everything_after_entry_segment(const __nonull t_entry *entry, \
				void *clone_after_entry_segment, size_t original_filesize)
{
	size_t		offset_after_entry = endian_8(entry->seg_hdr->p_offset) + endian_8(entry->seg_hdr->p_filesz);
	size_t		size_after_entry = original_filesize - offset_after_entry;
	void		*data_after_entry = safe(offset_after_entry, size_after_entry);

	if (data_after_entry == NULL)
		return (errors(ERR_CORRUPT, "failed to read after entry segment"));

	memcpy(clone_after_entry_segment, data_after_entry, size_after_entry);

	return (true);
}

/*
** ---------------------------------- move offset ------------------------------
*/

static void	move_offset_if_needed(Elf64_Off offset_after_entry, Elf64_Off *offset)
{
	size_t		vicious_payload_size = ((end_payload - begin_payload) + 8 - ((end_payload - begin_payload) % 8));
	Elf64_Off	actual_offset = endian_8(*offset);

	if (actual_offset > offset_after_entry)
		*offset = endian_8(actual_offset + vicious_payload_size);
}

static bool	move_every_offset_after_entry(t_clone *clone_map, t_entry *entry)
{
	size_t		offset_after_entry = endian_8(entry->seg_hdr->p_offset) + endian_8(entry->seg_hdr->p_filesz);

	move_offset_if_needed(offset_after_entry, &clone_map->elf64_hdr->e_phoff);
	// foreach segment
	{
		const Elf64_Half	phentsize = endian_2(clone_map->elf64_hdr->e_phentsize);
		Elf64_Half			phnum = endian_2(clone_map->elf64_hdr->e_phnum);
		char				(*segments)[phnum][phentsize] = clone_map->section_hdr_table;

		while (phnum--)
		{
			Elf64_Phdr *elf64_segment_hdr = (Elf64_Phdr *)(*segments)[phnum];
			move_offset_if_needed(offset_after_entry, &elf64_segment_hdr->p_offset);
			move_offset_if_needed(offset_after_entry, &elf64_segment_hdr->p_vaddr);
			move_offset_if_needed(offset_after_entry, &elf64_segment_hdr->p_paddr);
		}
	}

	if (clone_map->elf64_hdr->e_shoff)
	{
		move_offset_if_needed(offset_after_entry, &clone_map->elf64_hdr->e_shoff);
		// foreach section
		{
			const Elf64_Half	shentsize = endian_2(clone_map->elf64_hdr->e_shentsize);
			Elf64_Half			shnum = endian_2(clone_map->elf64_hdr->e_shnum);
			char				(*sections)[shnum][shentsize] = clone_map->section_hdr_table;

			while (shnum--)
			{
				Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];
				move_offset_if_needed(offset_after_entry, &elf64_section_hdr->sh_offset);
				move_offset_if_needed(offset_after_entry, &elf64_section_hdr->sh_addr);
			}
		}
	}
	return (true);
}

/*
** ---------------------------------- adjust sizes -----------------------------
*/

static bool	adjust_sizes(t_clone *clone_map, size_t vicious_payload_size, void *clone)
{
	t_entry	clone_entry;

	{// ugly tmp find entry seg and sect in clone
		const Elf64_Addr	entry_addr = endian_8(clone_map->elf64_hdr->e_entry);
		const Elf64_Off		phoff = endian_8(clone_map->elf64_hdr->e_phoff);
		const Elf64_Half	phentsize = endian_2(clone_map->elf64_hdr->e_phentsize);
		Elf64_Half			phnum = endian_2(clone_map->elf64_hdr->e_phnum);
		char				(*segments)[phnum][phentsize] = clone + phoff;
		while (phnum--)
		{
			Elf64_Phdr *elf64_seg_hdr = (Elf64_Phdr *)(*segments)[phnum];
			const Elf64_Addr seg_addr = endian_8(elf64_seg_hdr->p_vaddr);
			const Elf64_Xword seg_size = endian_8(elf64_seg_hdr->p_memsz);

			if (seg_addr <= entry_addr && entry_addr < seg_addr + seg_size)
			{
				clone_entry.seg_hdr = elf64_seg_hdr;
				break;
			}
		}
		const Elf64_Off		shoff = endian_8(clone_map->elf64_hdr->e_shoff);
		const Elf64_Half	shentsize = endian_2(clone_map->elf64_hdr->e_shentsize);
		Elf64_Half			shnum = endian_2(clone_map->elf64_hdr->e_shnum);
		char				(*sections)[shnum][shentsize] = clone + shoff;
		while (shnum--)
		{
			Elf64_Shdr *elf64_section_hdr = (Elf64_Shdr *)(*sections)[shnum];
			const Elf64_Addr section_addr = endian_8(elf64_section_hdr->sh_addr);
			const Elf64_Xword section_size = endian_8(elf64_section_hdr->sh_size);

			if (section_addr <= entry_addr && entry_addr < section_addr + section_size)
			{
				clone_entry.sect_hdr = elf64_section_hdr;
				break;
			}
		}
	}

	clone_entry.seg_hdr->p_filesz = endian_8(endian_8(clone_entry.seg_hdr->p_filesz) + vicious_payload_size);
	clone_entry.seg_hdr->p_memsz = endian_8(vicious_payload_size);
	if (clone_entry.sect_hdr)
		clone_entry.sect_hdr->sh_size = endian_8(endian_8(clone_entry.sect_hdr->sh_size) + vicious_payload_size);

	return (true);
}

/*
** ---------------------------------- clone skeleton ---------------------------
*/

static bool	create_clone_skeleton(t_clone *clone_map, Elf64_Ehdr *original_hdr)
{
	// copy elf header
	*clone_map->elf64_hdr = *original_hdr;

	// copy program header
	if (original_hdr->e_phoff == 0)
		return (errors(ERR_CORRUPT, "no program header found"));

	const size_t	ph_size = endian_2(original_hdr->e_phentsize) \
							* endian_2(original_hdr->e_phnum);
	const void		*original_ph = safe(endian_8(original_hdr->e_phoff), ph_size);

	clone_map->program_hdr_table = malloc(ph_size);
	if (clone_map->program_hdr_table == NULL)
		return (errors(ERR_SYS, "failed to allocated memory for clone phdr"));
	memcpy(clone_map->program_hdr_table, original_ph, ph_size);

	// copy section header (if any)
	if (original_hdr->e_shoff == 0)
		return (true);

	const size_t	sh_size = endian_2(original_hdr->e_shentsize) \
							* endian_2(original_hdr->e_shnum);
	const void		*original_sh = safe(endian_8(original_hdr->e_shoff), sh_size);
	clone_map->section_hdr_table = malloc(sh_size);
	if (clone_map->section_hdr_table == NULL)
		return (errors(ERR_SYS, "failed to allocated memory for clone shdr"));
	memcpy(clone_map->section_hdr_table, original_sh, sh_size);
	return (true);
}

static bool	apply_clone_skeleton(t_clone *clone_map, void *clone, t_entry *entry)
{
	const size_t	offset_after_entry = endian_8(entry->seg_hdr->p_offset) + endian_8(entry->seg_hdr->p_filesz);
	const size_t	ph_size = endian_2(clone_map->elf64_hdr->e_phentsize) \
							* endian_2(clone_map->elf64_hdr->e_phnum);
	const size_t	sh_size = endian_2(clone_map->elf64_hdr->e_shentsize) \
							* endian_2(clone_map->elf64_hdr->e_shnum);

	move_offset_if_needed(offset_after_entry, &clone_map->elf64_hdr->e_phoff);
	move_offset_if_needed(offset_after_entry, &clone_map->elf64_hdr->e_shoff);

	memcpy(clone + endian_8(clone_map->elf64_hdr->e_phoff), \
		clone_map->program_hdr_table, ph_size);
	if (clone_map->elf64_hdr->e_shoff)
		memcpy(clone + endian_8(clone_map->elf64_hdr->e_shoff), \
			clone_map->section_hdr_table, sh_size);

	free(clone_map->section_hdr_table);
	free(clone_map->program_hdr_table);

	return (true);
}

/*
** ---------------------------------- packer -----------------------------------
*/

bool	elf64_packer(__nonull void *clone, size_t original_filesize)
{
	Elf64_Ehdr		*original_hdr = safe(0, sizeof(Elf64_Ehdr));
	t_clone			clone_map = {clone, NULL, NULL};
	t_entry			entry;
	size_t			vicious_payload_size = ((end_payload - begin_payload) + 8 - ((end_payload - begin_payload) % 8));

// for readability purpose:
# define clone_after_entry_segment (clone + endian_8(entry.seg_hdr->p_offset) + endian_8(entry.seg_hdr->p_filesz) + vicious_payload_size)

	if (original_hdr == NULL)
		return (errors(ERR_CORRUPT, "invalid elf64 header"));

	if (!find_entry(&entry, original_hdr)
	|| !create_clone_skeleton(&clone_map, original_hdr)
	|| !copy_everything_before_entry_segment(&entry, clone)
	|| !add_payloaded_entry_segment(&entry, clone + endian_8(entry.seg_hdr->p_offset))
	|| !adjust_sizes(&clone_map, vicious_payload_size, clone)
	|| !copy_everything_after_entry_segment(&entry, clone_after_entry_segment, original_filesize)
	|| !move_every_offset_after_entry(&clone_map, &entry))
	{
		apply_clone_skeleton(&clone_map, clone, &entry);
		return (errors(ERR_THROW, __func__));
	}
	apply_clone_skeleton(&clone_map, clone, &entry);
	return (true);
}
