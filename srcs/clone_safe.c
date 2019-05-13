/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clone_safe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 03:15:48 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/13 20:30:43 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static struct
{
	void	*ptr;
	size_t	filesize;
}		safe_pointer = {NULL, 0};

/*
** safe()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

__warn_unused_result
void		*clone_safe(const size_t offset, const size_t size)
{
	if (offset + size > safe_pointer.filesize || offset + size < offset)
		return (NULL);
	return (safe_pointer.ptr + offset);
}

bool		alloc_clone(const size_t original_filesize)
{
	/* filesize + payload size + alignment */
	safe_pointer.filesize = original_filesize + 4096;
	safe_pointer.ptr = malloc(safe_pointer.filesize);

	if (safe_pointer.ptr == NULL)
		return (errors(ERR_SYS, "while allocating clone"));

	return (true);
}

void		free_clone(void)
{
	free(safe_pointer.ptr);
}

__nonull
bool		write_clone_file(void)
{
	int	fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

	if (fd == -1)
		return errors(ERR_SYS, "failed creating file " OUTPUT_FILENAME);

	if (write(fd, safe_pointer.ptr, safe_pointer.filesize) == -1)
	{
		close(fd);
		return errors(ERR_SYS, "failed writing to " OUTPUT_FILENAME);
	}

	close(fd);
	return (true);
}
