; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    decrypt.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/14 15:50:07 by agrumbac          #+#    #+#              ;
;    Updated: 2019/02/14 16:26:42 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

;void
;	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)

section .text
	global decrypt

decrypt:
	push rbp
	mov rbp, rsp
	push r12                   ; backup preserved registers
	push r13

	; | mut  | mut  | const | const | const      |
	; | rsi  | r13  | rdx   | rdi   | r12        |
	; | data | size | key   | delta | num_rounds |
	mov r12, rdi
	mov r13, rcx
	mov rdi, 0x9E3779B9        ; init constant delta

	jmp __while_size_ge_8

decipher:;---------------------; begin decipher subprocedure

	; (registers above are preserved and const)
	; | mut | const (* mut) | mut   | mut | mut | mut | mut |
	; | rax | rsi   ([rsi]) | rcx   | r8  | r9  | r10 | r11 |
	; | sum | v     ( v[] ) | count | ..  | ..  | v0  | v1  |
	mov r11, [rsi]
	mov r10, r11
	shr r11, 32                ; v0 = v[0]
	mov rax, 0xffffffff
	and r10, rax               ; v1 = v[1]
	mov rcx, r12               ; count = num_rounds
	mov rax, rdi
	imul rax, rcx              ; sum = delta * num_rounds

__loop_num_rounds:
	mov r8, r10
	shl r8, 4
	mov r9, r10
	shr r9, 5
	xor r8, r9
	add r8, r10                ; r8 = ((v0 << 4) ^ (v0 >> 5)) + v0
	mov r9, rax
	shr r9, 11
	and r9, 3
	shl r9, 2                  ; These concealed pointers,
	add r9, rdx                ; Oh abstraction from beyond !
	mov r9d, DWORD [r9]        ; Defenseless naked...
	add r9, rax                ; r9 = (sum + key[(sum>>11) & 3])
	xor r8, r9
	sub r11d, r8d              ; v1 -= (r8 ^ r9)

	sub rax, rdi               ; sum -= delta

	mov r8, r11
	shl r8, 4
	mov r9, r11
	shr r9, 5
	xor r8, r9
	add r8, r11                ; r8 = ((v1 << 4) ^ (v1 >> 5)) + v1
	mov r9, rax
	and r9, 3
	shl r9, 2                  ; Refined typed arrays,
	add r9, rdx                ; Forever unwavering...
	mov r9d, DWORD [r9]        ; Save us from the void !
	add r9, rax                ; r9 = (sum + key[sum & 3])
	xor r8, r9
	sub r10d, r8d              ; v0 -= (r8 ^ r9)

	dec rcx                    ; loop until count reaches 0
	test rcx, rcx
	jnz __loop_num_rounds
	                           ; load result back in v
	mov rax, r11               ; [.... 0000] rax
	shl rax, 32                ; [0000 ....] rax
	mov rcx, 0xffffffff
	and r10, rcx               ; [.... 1111] r10
	or  rax, r10               ; [0000 1111] rax | r10
	mov [rsi], rax             ; store result in v
;------------------------------; end decipher
	add rsi, 8
	sub r13, 8
__while_size_ge_8:
	cmp r13, 8
	jge decipher

	pop r13                    ; restore preserved registers
	pop r12

	leave
	ret
