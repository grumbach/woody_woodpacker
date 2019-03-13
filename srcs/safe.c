/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/03/13 05:28:31 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static struct	s_safe_pointer
{
	void		*ptr;
	size_t		filesize;
}				safe_pointer = {NULL, 0};

/*
** safe()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

void					*safe(const Elf64_Off offset, const size_t size)
{
	return ((void *) ((size_t)(safe_pointer.ptr + offset) * \
		(offset + size <= safe_pointer.filesize)));
}

size_t					read_file(const char *filename)
{
	int					fd;
	void				*ptr;
	struct stat			buf;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (errors(ERR_SYS, "open failed"));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_SYS, "fstat failed"));
	if (buf.st_mode & S_IFDIR)
		return (errors(ERR_USAGE, "can't parse directories"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "mmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));

	safe_pointer.ptr = ptr;
	safe_pointer.filesize = buf.st_size;
	return (safe_pointer.filesize);
}

void					free_file(void)
{
	if (munmap(safe_pointer.ptr, safe_pointer.filesize))
		errors(ERR_SYS, "munmap failed");
}
