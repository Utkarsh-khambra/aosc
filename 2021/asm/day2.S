
%define BUFFER_SIZE 1023 

global main
extern fopen
extern fread
extern fclose
extern printf

section .text

; Find char dl in [rdi, rdi+rsi)
find:
    mov rcx, rsi
    mov rax, 0
find_loop:
    cmp dl, byte[rdi]
    je end_find_loop
    inc rdi
    inc rax
    loop find_loop
end_find_loop:
    ret

is_digit:
    mov al, 0
    mov cl, 0
    cmp dil, "0"
    setge al
    cmp dil, "9"
    setle cl
    and al, cl
    movzx rax, al
    ret


; Tries to get any integer it can find and sets
; out to end of integer digit, or end of str 
; if no integer
; "1243gagd" would give 1234 and out will be at g
; "ad1243gagd" would give 1234 and out will be at g
; bool parse_integer(const char* str,  char** out, int* integer)
; returns true if found an integer else false

;ERROR Unaligned stack somewhere
before_jumping_up:
    pop rdi
    inc rdi
    push rdi
    cmp byte[rdi], 0
    je parse_integer_end
    jmp first_digit

parse_integer:
    push r15
    push rsi
    push rdx
    mov r8, 0
    push r8
    push rdi
first_digit:
    mov dil, byte[rdi]
    call is_digit
    cmp rax, 0
    je before_jumping_up
    pop rdi
    pop r8
    mov r8, 1 ; TODO You can use mov [rsp+8], 1
    push r8
    mov r15, 0
parse_integer_start:
    imul r15, 10
    mov al, byte[rdi]
    sub al, "0"
    movzx rax, al
    add r15, rax 
    inc rdi
    push rdi
    mov dil, byte[rdi]
    call is_digit
    cmp rax, 0
    je parse_integer_end
    pop rdi
    jmp parse_integer_start
parse_integer_end:
    pop rdi
    pop r8
    cmp r8, 1
    sete al
    movzx rax, al
    pop rdx
    pop rsi
    mov [rsi], rdi
    mov [rdx], r15
    pop r15
    ret




; Returns buffer to be filled again
; const char* count_forward_and_down(const char* buffer, int* forward, int* depth, int* part2_depth)
; TODO make buffer end with NULL to get read of length parameter
count_forward_and_down:
    push 0 ; to align stack
    push r12
    push r13
    push r14
    push r15

    mov r12, rsi ; Save since further function calls may corrupt these
    mov r13, rdx ; Save since further function calls may corrupt these
    mov r14, rcx ; Save since further function calls may corrupt these
count_start:
    mov dl, byte[rdi]
    cmp dl, "f"
    je parse_forward
    cmp dl, "d"
    je parse_down
    cmp dl, "u"
    je parse_up
    cmp dl, 0
    je end_count_forward_and_down

parse_forward:
    push 0 ; outvalue
    push 0 ; out array
    ; push rdi
    lea rsi, [rsp]
    lea rdx, [rsp+8]
    call parse_integer
    cmp rax, 0
    je cannot_parse_int
    ; pop rdi
    pop rdi
    mov rdi, [rdi]
    pop rax
    add [r12], rax 
    imul eax,dword[aim]
    mov dword[aim], eax
    mov eax, dword[r14]
    add eax, dword[aim]
    mov dword[r14], eax
    inc rdi
    jmp count_start
parse_up:
    push 0 ; outvalue
    push 0 ; out array
    ; push rdi
    lea rsi, [rsp]
    lea rdx, [rsp+8]
    call parse_integer
    cmp rax, 0
    je cannot_parse_int
    ; pop rdi
    pop rdi
    mov rdi, [rdi]
    pop rax
    sub [r13], rax 
    sub [aim], rax
    inc rdi
    jmp count_start
parse_down:
    push 0 ; outvalue
    push 0 ; out array
    ; push rdi
    lea rsi, [rsp]
    lea rdx, [rsp+8]
    call parse_integer
    cmp rax, 0
    je cannot_parse_int
    ; pop rdi
    pop rdi
    mov rdi, [rdi]
    pop rax
    add [r13], rax 
    add [aim], rax
    inc rdi
    jmp count_start

end_count_forward_and_down:
; TODO set rax properly
    mov rax, rdi
    pop r12
    pop r15
    pop r14
    pop r13
    pop r12
    ret

main:
    push 0 ; total_forward
    push 0 ; total_depth
    push 0 ; part2_depth
    mov rdi, filename 
    mov rsi, file_mode
    call fopen ; Past three pushes made stack 16 byte aligned here
    cmp rax, 0
    je cannot_open_file
    mov [file_handle], rax
    mov rdi, my_buffer
    mov rdx, BUFFER_SIZE
    mov byte[my_buffer+BUFFER_SIZE], 0
    push rbx
    mov rbx, 0
read_and_parse:
    mov r12, rdi
    mov rsi, 1 
    mov rcx, [file_handle]
    call fread
    cmp rax, rdx
    jl either_eof_or_error ; Keeps rax intact
    jmp fix_partial_line
detour:
    mov rdi, r12
    lea rsi, [rsp +16] 
    lea rdx, [rsp + 8] 
    lea rcx,  [rsp] 
    call count_forward_and_down
    cmp byte[rax], 0 ; parsed everything no need to do anything
    je done
    mov rdi, rax
    sub rax, my_buffer ; Get the new buffer length
    mov rdx, BUFFER_SIZE
    sub rdx, rax
    inc rbx
    cmp rbx, 2
    je done
    ; jmp read_and_parse

    ; push 0 ; None
    ; push 0
    ; push 1
    ; mov rdi, test_str
    ; mov rsi, rsp
    ; lea rdx, [rsp+8]
    ; call parse_integer 
    ; pop rsi
    ; pop rdi
    ; mov rdi, fmt
done:
    ; pop rbx
    ; pop rsi
    ; pop rdx
    ; pop rcx
    ; xor rax, rax
    ; mov rdi, fmt_result
    ; call printf
    ; pop rdi
    mov rax, 0
    ret

either_eof_or_error: ; TODO handle this case

; try to keep registers intact
fix_partial_line:
    push rdi
    push rcx
    mov rcx, BUFFER_SIZE-1
    lea rdi, [my_buffer+rcx]
fix_partial_line_start:
    cmp byte[rdi], 10
    je fix_partial_line_impl
    dec rdi
    loop fix_partial_line_start
fix_partial_line_impl:
    mov byte[rdi],0
    pop rcx
    pop rdi
    jmp detour 

cannot_open_file:
   mov rdi, err
   mov rsi, error_1
   xor rax,rax
   call printf
   jmp done

cannot_parse_int:
    jmp done

section .data

filename: db "input/day2", 0
file_mode: db "r", 0
fmt_result: db "%d %d %d", 10, 0
fmt: db "%d", 10, 0
err: db "%s", 10, 0
error_1: db "Cannot open file", 0 
test_str: db "aadg", 0
file_handle: dq 0
aim: dw 0 ; To keep track between the calls

section .bss

my_buffer: resb BUFFER_SIZE+1
