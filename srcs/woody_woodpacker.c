/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/10 20:25:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

int		main(int ac, char **av)
{
	Elf64_Ehdr		*elf64_hdr;

	if (ac != 2)
	{
		dprintf(2, "usage: %s elf64_file\n", av[0]);
		return (EXIT_FAILURE);
	}
	read_file(av[1]);

	// check file
	elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (check_eligibility(elf64_hdr) == false)
		goto exit_failure;
	// set endian for the future
	endian_big_mode(elf64_hdr->e_ident[EI_DATA] == ELFDATA2MSB);


	__unused const Elf64_Shdr *entry_section = get_entry_section(elf64_hdr);
	// entry_section = compress_entry(entry_section);
	// entry_section = encrypt_entry(entry_section);

	// packer
	// new_file = pack_in_new_bin(unpacker_code, entry_section);
	// write_to_file(new_file);


	free_file();
	return (EXIT_SUCCESS);

exit_failure:
	free_file();
	return (EXIT_FAILURE);
}
