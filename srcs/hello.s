; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    hello.s                                            :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/11 14:08:33 by agrumbac          #+#    #+#              ;
;    Updated: 2019/02/11 14:53:34 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define SYSCALL_WRITE	0x1
%define STDOUT			0x1

section .text
	global say_hello
	global end_hello

say_hello:
	push rbp
	mov rbp, rsp

	mov rax, 0x00000a6f6c6c6568
	sub rsp, 8
	push rax

	mov rdi, STDOUT
	mov rsi, rsp
	mov rdx, 8

	mov rax, SYSCALL_WRITE
	syscall

	pop rdi
	add rsp, 8

	leave
	ret
end_hello:
