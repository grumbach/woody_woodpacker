/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 19:29:40 by agrumbac          #+#    #+#             */
/*   Updated: 2019/04/19 17:15:14 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"
#include <errno.h>

bool			errors(const int err, const char *str)
{
	static const char	*msg[ERR_NUMBER] =
	{
		[ERR_SYS]	= "\033[31m[FATAL ERROR] \033[0m",
		[ERR_THROW]	= "  -> in _",
		[ERR_USAGE]	= "Bad usage: ",
		[ERR_CORRUPT]	= "\033[33m[WARNING] \033[0m" "Corrupt file: ",
	};

	if (err == ERR_SYS)
		dprintf(2, "%s%s: %s\n", msg[err], str, strerror(errno));
	else
		dprintf(2, "%s%s\n", msg[err], str);

	return (false);
}
