/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_setup_payload.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 00:10:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/11 11:25:48 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64_private.h"
#include <time.h>

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
}__attribute__((packed));

static void	generate_key(char *buffer, size_t len)
{
	srand(time(NULL));

	for (size_t i = 0; i < len; i++)
		buffer[i] = rand();
}

static bool	init_constants(struct payload_constants *constants, \
			const struct entry *original_entry)
{
	memcpy(constants->key, SECRET_SIGNATURE, SECRET_LEN);
	generate_key((char *)constants->key + SECRET_LEN, 16 - SECRET_LEN);

	constants->relative_pt_load_address = original_entry->section_end_offset - endian_8(original_entry->safe_phdr->p_offset);
	constants->pt_load_size             = endian_8(original_entry->safe_phdr->p_memsz);
	constants->relative_text_address    = original_entry->section_end_offset - endian_8(original_entry->safe_shdr->sh_offset);
	constants->relative_entry_address   = constants->relative_text_address - original_entry->offset_in_section;

	return true;
}

bool		setup_payload(void *clone, const struct entry *original_entry)
{
	struct payload_constants	constants;

	if (init_constants(&constants, original_entry) == false)
		return errors(ERR_THROW, "setup_payload");

	const size_t	payload_size = end_payload - begin_payload;
	const size_t	text_size    = constants.relative_text_address;
	void	*payload_location    = clone + original_entry->section_end_offset;
	void	*constants_location  = payload_location + CALL_INSTR_SIZE;
	void	*text_location       = payload_location - text_size;

	memcpy(text_location - original_entry->offset_in_section, begin_payload, payload_size);//TMP WTF

	// encrypt(32, text_location, constants.key, text_size);
	// memcpy(payload_location, begin_payload, payload_size);
	// memcpy(constants_location, &constants, sizeof(constants));

	return true;
}
