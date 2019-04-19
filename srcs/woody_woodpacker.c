/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/04/19 17:18:58 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static const t_format	implemented_formats[FMT_SIZE] =
{
	[FMT_ELF64] = {elf64_identifier, elf64_packer}
	// [FMT_ELF32] = {elf32_identifier, elf32_packer},
	// [FMT_MACHO64] = {macho64_identifier, macho64_packer},
	// [FMT_MACHO32] = {macho32_identifier, macho32_packer},
	// [FMT_PE64] = {pe64_identifier, pe64_packer},
	// [FMT_PE32] = {pe32_identifier, pe32_packer},
};

/*
**
*/

static inline size_t	detect_format(void)
{
	for (size_t i = 0; i < FMT_SIZE; i++)
	{
		if (implemented_formats[i].format_identifier())
			return (i);
	}
	return (FMT_SIZE);
}

static inline bool	write_clone_file(__nonull void *clone, size_t clone_size)
{
	int		fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY, S_IRWXU);

	if (fd == -1)
	{
		errors(ERR_SYS, "failed creating file " OUTPUT_FILENAME);
		return (false);
	}
	if (write(fd, clone, clone_size) == -1)
	{
		close(fd);
		errors(ERR_SYS, "failed writing to " OUTPUT_FILENAME);
		return (false);
	}
	close(fd);
	return (true);
}

int			main(int ac, char **av)
{
	void		*clone = NULL;
	size_t		filesize;
	size_t		format;
	size_t		clone_size;

	if (ac != 2)
		return (dprintf(2, "usage: %s <executable>\n", av[0]), EXIT_FAILURE);

	filesize = read_file(av[1]);
	if (filesize == 0)
		return (EXIT_FAILURE);

	format = detect_format();
	if (format == FMT_SIZE)
	{
		dprintf(2, "%s: %s is not a valid input file\n", av[0], av[1]);
		goto exit_failure;
	}

	clone_size = filesize + 4096; // TODO payload size
	clone = malloc(clone_size);
	if (clone == NULL)
	{
		errors(ERR_SYS, "while allocating clone");
		goto exit_failure;
	}

	if (implemented_formats[format].packer(clone, filesize) == false)
	{
		dprintf(2, "%s: file corruption detected in %s, aborting.\n", av[0], av[1]);
		goto exit_failure;
	}

	if (write_clone_file(clone, clone_size) == false)
		goto exit_failure;

	printf("\e[32mSuccessfully packed\e[33m %s \e[32min \e[33m" OUTPUT_FILENAME "\e[32m!\e[0m\n", av[1]);

	free(clone);
	free_file();
	return (EXIT_SUCCESS);

exit_failure:
	free(clone);
	free_file();
	return (EXIT_FAILURE);
}
