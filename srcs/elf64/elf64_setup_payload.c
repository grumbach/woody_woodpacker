/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_setup_payload.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 00:10:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/15 19:29:57 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"
#include <time.h>

/*
**  Elf64_packer memory overview
**  ============================
**
**         | mem address | file offset | payload rel address      |
**         | ----------- | ----------- | ------------------------ |
**         | p_vaddr     | p_offset    | relative_pt_load_address |
**         |             |             |                          |
**         | sh_addr     | sh_offset   | relative_text_address    |
**         |             |             |                          |
**         | e_entry     |             | relative_entry_address   |
**
**                          .        .
**                          |  ...   |
**              p_offset -> |========| PT_LOAD  - relative_pt_load_address
**                          |  ...   |          ^
**                          |  ...   |          |
**             sh_offset -> |--------| .Text    |- relative_text_address
**                      |   |  ...   |          |^
**                      V   |  ...   |          ||
**    offset_in_section -   |>>>>>>>>| entry    ||- relative_entry_address
**                          |  ...   |          ||^
**                          |--------|          |||
**                          |  ...   |          |||
**                          |  ...   |          |||
**                          |  ...   |          |||
**   end_of_last_section -> |@@@@@@@@| payload  |||
**                          |@      @| |
**                          |@      @| V
**                          |@@@@@@@@| payload_size
**                          |========|
**                          |  ...   |
**                          .        .
*/

# define CALL_INSTR_SIZE	5 /* sizeof "call mark_below" -> e8 2000 0000 */
# define SECRET_SIGNATURE	"42Remblai"
# define SECRET_LEN		sizeof(SECRET_SIGNATURE)

struct payload_constants
{
	uint32_t	key[4];
	uint64_t	relative_pt_load_address;
	uint64_t	pt_load_size;
	uint64_t	relative_text_address;
	uint64_t	relative_entry_address;
	uint64_t	text_size;
}__attribute__((packed));

static void	generate_key(char *buffer, size_t len)
{
	srand(time(NULL));

	for (size_t i = 0; i < len; i++)
		buffer[i] = rand();
}

static void	init_constants(struct payload_constants *constants, \
			const struct entry *original_entry)
{
	memcpy(constants->key, SECRET_SIGNATURE, SECRET_LEN);
	generate_key((char *)constants->key + SECRET_LEN, 16 - SECRET_LEN);

	const size_t		end_of_last_section = original_entry->end_of_last_section;
	const Elf64_Off		p_offset  = endian_8(original_entry->safe_phdr->p_offset);
	const Elf64_Xword	p_memsz   = endian_8(original_entry->safe_phdr->p_memsz);
	const Elf64_Off		sh_offset = endian_8(original_entry->safe_shdr->sh_offset);
	const size_t		sh_size   = endian_8(original_entry->safe_shdr->sh_size);

	constants->relative_pt_load_address = end_of_last_section - p_offset;
	constants->pt_load_size             = p_memsz;
	constants->relative_text_address    = end_of_last_section - sh_offset;
	constants->relative_entry_address   = constants->relative_text_address - original_entry->offset_in_section;
	constants->text_size                = sh_size;
}

bool		setup_payload(const struct entry *original_entry)
{
	struct payload_constants	constants;

	init_constants(&constants, original_entry);

	const size_t	payload_size = end_payload - begin_payload;
	const size_t	text_size    = endian_8(original_entry->safe_shdr->sh_size);
	const size_t	payload_off  = original_entry->end_of_last_section;
	const size_t	text_off     = payload_off - constants.relative_text_address;

	void	*payload_location    = clone_safe(payload_off, payload_size);
	void	*constants_location  = clone_safe(payload_off + CALL_INSTR_SIZE, sizeof(constants));
	void	*text_location       = clone_safe(text_off, text_size);

	if (!payload_location || !constants_location || !text_location)
		return (errors(ERR_CORRUPT, "wildly unreasonable"));

	encrypt(32, text_location, constants.key, text_size);
	memcpy(payload_location, begin_payload, payload_size);
	memcpy(constants_location, &constants, sizeof(constants));

	return true;
}
