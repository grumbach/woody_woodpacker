/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/11 16:23:42 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void	say_hello(void);
void	end_hello(void);

int		main(int ac, char **av)
{
	Elf64_Ehdr		*elf64_hdr;
	size_t			filesize;

	if (ac != 2)
	{
		dprintf(2, "usage: %s elf64_file\n", av[0]);
		return (EXIT_FAILURE);
	}
	filesize = read_file(av[1]);

	// check file
	elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (check_eligibility(elf64_hdr) == false)
		goto exit_failure;
	// set endian for the future
	endian_big_mode(elf64_hdr->e_ident[EI_DATA] == ELFDATA2MSB);

	// create new 'clone' file
	void			*clone = malloc(filesize);
	void			*original = safe(0, filesize);
	if (original == NULL || clone == NULL)
		fatal("we're doomed...");
	memcpy(clone, original, filesize);

	// change clone's entry
	Elf64_Off			entry_offset_in_sect;
	const Elf64_Shdr	*entry_section = get_entry_section(elf64_hdr, &entry_offset_in_sect);
	if (entry_section == NULL)
	{
		dprintf(2, "entry point is not in a section");
		goto exit_failure;
	}
	const Elf64_Off		code_entry = endian_8(entry_section->sh_offset) + entry_offset_in_sect;
	memcpy(clone + code_entry, say_hello, end_hello - say_hello);

	// write clone to new file
	int fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY, S_IRWXU);
	if (fd == -1)
		fatal("failed creating file " OUTPUT_FILENAME);
	if (write(fd, clone, filesize) == -1)
		fatal("failed writing to " OUTPUT_FILENAME);

	// entry_section = encrypt_entry(entry_section);
	// packer
	// new_file = pack_in_new_bin(unpacker_code, entry_section);
	// write_to_file(new_file);

	close(fd);
	free(clone);
	free_file();
	return (EXIT_SUCCESS);

exit_failure:
	free_file();
	return (EXIT_FAILURE);
}
