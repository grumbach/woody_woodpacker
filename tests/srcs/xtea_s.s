; ------------------- xtea encipher -----------------
; void    s_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4])
section .text
	global s_encipher


s_encipher:
	push rbp
	mov rbp, rsp
	                           ; put everything in the right place
	                           ; rax   rdi    rsi   rdx   rcx    r8   r9   r10  r11
	                           ; sum   delta  v     key   count  ..   ..   v0   v1
	mov rcx, rdi               ; count = num_rounds
	mov r11, [rsi]
	mov r10, r11
	shr r11, 32                ; v0 = v[0]
	mov rax, 0xffffffff
	and r10, rax               ; v1 = v[1]
	mov rdi, 0x9E3779B9        ; delta
	xor rax, rax               ; sum = 0

__loop_e:
	mov r8, r11
	shl r8, 4
	mov r9, r11
	shr r9, 5
	xor r8, r9
	add r8, r11                ; r8 = ((v1 << 4) ^ (v1 >> 5)) + v1
	mov r9, rax
	and r9, 3
	shl r9, 2                  ; Those sweet C arrays...
	add r9, rdx                ; Oh sugar to our eyes !
	mov r9d, DWORD [r9]        ; Far off deep below...
	add r9, rax                ; r9 = (sum + key[sum & 3])
	xor r8, r9
	add r10d, r8d              ; v0 += (r8 ^ r9)

	add rax, rdi               ; sum += delta

	mov r8, r10
	shl r8, 4
	mov r9, r10
	shr r9, 5
	xor r8, r9
	add r8, r10                ; r8 = ((v0 << 4) ^ (v0 >> 5)) + v0
	mov r9, rax
	shr r9, 11
	and r9, 3
	shl r9, 2                  ; Pretty square brackets !
	add r9, rdx                ; Syntactic prose of heavens,
	mov r9d, DWORD [r9]        ; They come from above...
	add r9, rax                ; r9 = (sum + key[(sum>>11) & 3])
	xor r8, r9
	add r11d, r8d              ; v1 += (r8 ^ r9)

	dec rcx                    ; loop until count reaches 0
	test rcx, rcx
	jnz __loop_e
	                           ; load result back in v
	mov rax, r11               ; [.... 0000] rax
	shl rax, 32                ; [0000 ....] rax
	mov rcx, 0xffffffff
	and r10, rcx               ; [.... 1111] r10
	or  rax, r10               ; [0000 1111] rax | r10
	mov [rsi], rax             ; store result in v

	leave
	ret

; ------------------- xtea decipher -----------------
; void    decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4])
section .text
	global s_decipher

s_decipher:
	push rbp
	mov rbp, rsp

	                           ; put everything in the right place
	                           ; rax   rdi    rsi   rdx   rcx    r8   r9   r10  r11
	                           ; sum   delta  v     key   count  ..   ..   v0   v1
	mov rcx, rdi               ; count = num_rounds
	mov r11, [rsi]
	mov r10, r11
	shr r11, 32                ; v0 = v[0]
	mov rax, 0xffffffff
	and r10, rax               ; v1 = v[1]
	mov rdi, 0x9E3779B9        ; delta
	mov rax, rdi
	imul rax, rcx              ; sum = delta * num_rounds

__loop_d:
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
	jnz __loop_d
	                           ; load result back in v
	mov rax, r11               ; [.... 0000] rax
	shl rax, 32                ; [0000 ....] rax
	mov rcx, 0xffffffff
	and r10, rcx               ; [.... 1111] r10
	or  rax, r10               ; [0000 1111] rax | r10
	mov [rsi], rax             ; store result in v

	leave
	ret
