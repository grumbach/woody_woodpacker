; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    encryption.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/11 17:32:43 by agrumbac          #+#    #+#              ;
;    Updated: 2019/02/11 18:20:26 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

;void	encipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);
;void	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)
section .text
	global encrypt
	extern encipher

encrypt:
	push rbp
	mov rsp, rbp
	                         ; rdi        rsi  rdx  rcx
	                         ; num_rounds data key  size
	jmp __while_size_ge_8_ec
__loop_ec:
	push rdi
	push rcx
	call encipher
	pop rcx
	pop rdi
	add rsi, 8
	sub rcx, 8
__while_size_ge_8_ec:
	cmp rcx, 8
	jge __loop_ec

	leave
	ret

;void	decipher(uint num_rounds, uint32_t v[2], uint32_t const key[4]);
;void	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)
section .text
	global decrypt
	extern decipher

decrypt:
	push rbp
	mov rsp, rbp
	                         ; rdi        rsi  rdx  rcx
	                         ; num_rounds data key  size
	jmp __while_size_ge_8_dc
__loop_dc:
	push rdi
	push rcx
	call decipher
	pop rcx
	pop rdi
	add rsi, 8
	sub rcx, 8
__while_size_ge_8_dc:
	cmp rcx, 8
	jge __loop_dc

	leave
	ret
