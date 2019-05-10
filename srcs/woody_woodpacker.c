/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/10 03:13:34 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include "elf64.h"             /* for [FMT_ELF64] entry */

static const struct format	implemented_formats[FMT_SIZE] =
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

static size_t		detect_format(void)
{
	for (size_t i = 0; i < FMT_SIZE; i++)
	{
		if (implemented_formats[i].format_identifier())
			return (i);
	}
	return (FMT_SIZE);
}

__nonull
static bool		write_clone_file(void *clone, size_t clone_size)
{
	int		fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY, S_IRWXU);

	if (fd == -1)
		return errors(ERR_SYS, "failed creating file " OUTPUT_FILENAME);

	if (write(fd, clone, clone_size) == -1)
	{
		close(fd);
		return errors(ERR_SYS, "failed writing to " OUTPUT_FILENAME);
	}
	close(fd);
	return (true);
}

int			main(int ac, char **av)
{
	int		ret		= EXIT_SUCCESS;
	void		*clone		= NULL;
	size_t		filesize;
	size_t		clone_size;
	enum e_format	format;

	if (ac != 2)
	{
		errors(ERR_USAGE, "\nusage: %s <executable>\n", av[0]);
		goto exit_failure;
	}

	filesize = read_file(av[1]);
	if (filesize == 0)
		goto exit_failure;

	format = detect_format();
	if (format == FMT_SIZE)
	{
		errors(ERR_USAGE, "%s: %s is not a valid input file\n", av[0], av[1]);
		goto exit_failure;
	}

	clone_size = filesize + 4096; /* filesize + payload size + align */
	clone = malloc(clone_size);
	if (clone == NULL)
	{
		errors(ERR_SYS, "while allocating clone");
		goto exit_failure;
	}

	if (implemented_formats[format].packer(clone, clone_size) == false)
	{
		errors(ERR_CORRUPT, "%s: file corruption detected in %s, aborting.\n", av[0], av[1]);
		goto exit_failure;
	}

	if (write_clone_file(clone, clone_size) == false)
		goto exit_failure;

	printf("\e[32mSuccessfully packed\e[33m %s \e[32min \e[33m" OUTPUT_FILENAME "\e[32m!\e[0m\n", av[1]);

exit:
	free(clone);
	free_file();
	return (ret);
exit_failure:
	ret = EXIT_FAILURE;
	goto exit;
}
