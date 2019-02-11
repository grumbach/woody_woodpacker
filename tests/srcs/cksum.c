/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cksum.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 16:43:32 by agrumbac          #+#    #+#             */
/*   Updated: 2019/02/11 16:54:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xtea_test.h"

/*
** Muhahahahahaha! >:D-
** (don't do this at home)
*/

uint16_t		cksum(__unused const void *buffer, __unused size_t size)
{
	asm volatile (".intel_syntax;\n"
			"  xor rax, rax\n"
			"  cmp rsi, 0x1\n"
			"  jne _loop\n"
			"_odd_case:\n"
			"  xor rdx, rdx\n"
			"  mov dl , BYTE PTR [rdi]\n"
			"  sub rsi, 1\n"
			"  jmp _add_cksum\n"
			"_loop:\n"
			"  xor rdx, rdx\n"
			"  mov dx, WORD PTR [rdi]\n"
			"  add rdi, 2\n"
			"  sub rsi, 2\n"
			"_add_cksum:\n"
			"  add rax, rdx\n"
			"  cmp rsi, 1\n"
			"  jg _loop\n"
			"  je _odd_case\n"
			"  mov rcx, rax\n"
			"  shr rcx, 16\n"
			"  and rax, 0xffff\n"
			"  add rax, rcx\n"
			"  mov rcx, rax\n"
			"  shr rcx, 16\n"
			"  add rax, rcx\n"
			"  not rax\n"
			"  leave\n"
			"  ret\n");

	__builtin_unreachable();
}
