    global main
    extern fopen
    extern puts
    extern fread
    extern memcpy
    extern realloc
    extern fclose
    extern malloc
    extern feof
    extern printf
    extern free

    section .text

main:
    ; TODO Try to use appropriate width registers
    ; Allocate space to read file
    mov rdi, 1024
    mov rax, 1024
    mul rdi
    mov qword[buffer_size], rax 
    mov rdi, rax
    call malloc
    cmp rax, 0
    je print_error
    mov qword[buffer], rax

    ; Open file
    mov rdi, filename
    mov rsi, file_mode
    call fopen 
    cmp rax, 0
    je print_error
    mov qword [file_handle], rax

    mov rdi, qword[buffer]
    mov rsi, 1
    mov rdx, [buffer_size]
    ; mov rsi, 10
    mov rcx, qword[file_handle]
    call fread
    cmp rax, [buffer_size]
    jl check_fread_return
continue_parsing:
    mov rdi, [buffer]
    mov byte[rdi+rsi], 0 ; TODO check that rsi is not greater than buffer_size
    mov rcx, 0 ; Number of lines
    push rdi
    push rcx

    mov rdi, qword[file_handle]
    call fclose

    pop rcx
    pop rdi

count_lines:
    cmp byte[rdi], 10 ;compare with newline
    je increment_line_count
    cmp byte[rdi], 0
    je done_counting_line
    inc rdi
    jmp count_lines
done_counting_line:
    ; Print number of lines
    mov dword[nums_size], ecx
    imul ecx, 4
    mov edi, ecx 
    ; mov edi, fmt
    ; mov esi, dword[nums_size]
    ; xor rax, rax
    ; call printf
    call malloc
    cmp rax, 0
    je print_error
    mov qword[nums], rax
    ; Convert string to integers
    mov rdi, [buffer]
    mov r9, [nums]
    mov rax, 0 ; Partially parsed integer
    mov r15, 0;
extract_integers:
    mov sil, byte[rdi]
    cmp sil, 48 ; compare with '0'
    jge maybe_digit
    cmp sil, 10 ; compare with '\n'
    je push_integer_to_array
    cmp sil, 0
    jne print_integer_parsing_error

    mov rdi, [nums]
    mov esi, dword[rdi] ;first
    add rdi,4
    mov rcx, [nums_size] ; counter
    dec rcx
    mov rax, 0
start_loop:
    cmp dword[rdi], esi
    setg dl
    movzx rdx, dl
    add rax, rdx
    mov esi, dword[rdi]
    mov dl, 0
    add rdi, 4
    loop start_loop

    mov rdi, fmt
    mov rsi, rax
    mov rax, 0
    call printf

    ; Part 2
    mov edi, [nums]
    mov rcx, [nums_size]
    sub rcx, 2 ;TODO Why
part2:
    mov eax, dword[rdi]
    add rdi, 4
    add eax, dword[rdi]
    add rdi, 4
    add eax, dword[rdi]
    lea rdi, [rdi - 4*2]
    mov dword[rdi], eax
    lea rdi, [rdi+4]
    loop part2

    sub dword[nums_size], 2
    mov rdi, [nums]
    mov esi, dword[rdi] ;first
    add rdi,4
    mov rcx, [nums_size] ; counter
    dec rcx
    mov rax, 0
part2_sumation_loop:
    cmp dword[rdi], esi
    setg dl
    movzx rdx, dl
    add rax, rdx
    mov esi, dword[rdi]
    mov dl, 0
    add rdi, 4
    loop part2_sumation_loop

    mov rdi, fmt
    mov rsi, rax
    mov rax, 0
    call printf


done:

    mov rdi, [nums]
    cmp rdi, 0
    jne free_nums
    ret
free_nums:
    call free
    mov rdi, [buffer]
    cmp rdi, 0
    jne free_buffer
free_buffer:
    call free
    ret

push_integer_to_array:
   mov dword[r9], eax 
   mov rax, 0
   lea r9, [r9+4]
   inc rdi
   inc r15;
   jmp extract_integers

maybe_digit:
    cmp sil, 57 ; compare with '9'
    jle found_digit
    jmp print_integer_parsing_error

found_digit:
    imul rax, 10 
    sub sil, 48
    movzx rsi, sil
    add rax, rsi
    inc rdi
    jmp extract_integers

print_integer_parsing_error:
    ; push rax
    mov rdi, integer_parsing_error
    call puts
    ; pop rax
    mov rdi, fmt
    mov rsi, rax
    mov rax, 0
    call printf 
    jmp done

increment_line_count:
    inc rcx;
    inc rdi
    jmp count_lines

print_error:
    mov rdi, error 
    call puts
    jmp done

check_fread_return:
    mov rdi, qword[file_handle]    
    push r12
    mov r12, rax
    call feof
    cmp rax, 0
    je print_error
    mov rsi, r12
    pop r12
    jmp continue_parsing

    section .data

filename:
    db "input/day1", 0
file_mode:
    db "r", 0
error:
    db "Error happened",0
fmt:
    db "%d",10, 0
integer_parsing_error:
    db "Parsing of integer failed, found unexpected character", 10, 0

file_handle: dq 0
buffer: dq 0
buffer_size: dq 0
nums: dq 0
nums_size: dw 0

