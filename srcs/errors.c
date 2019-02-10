/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 19:29:40 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/10 15:59:25 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

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
