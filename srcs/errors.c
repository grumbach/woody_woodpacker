/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 19:29:40 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/02 09:58:38 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

# define WOODY_FATAL				"\033[31m[FATAL ERROR] \033[0m"
# define WOODY_WARN				"\033[33m[WARNING] \033[0m"

__noreturn
void			fatal(const char * const message)
{
	dprintf(2, WOODY_FATAL);
	perror(message);

	exit(EXIT_FAILURE);
}

void			warn(const char * const message)
{
	dprintf(2, WOODY_WARN);
	perror(message);
}
