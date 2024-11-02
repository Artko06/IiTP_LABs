global bst           ; метка bst будет видимой извне
 
section .data 
    bufferInput: times 21 db 0, 0Ah
    bufferInt db 21 dup(0)      
    bufferFileNum db 21 dup(0) 

    msRoot: db "Enter value root in range from -9 223 372 036 854 775 808 until 9 223 372 036 854 775 807: ", 0 ; 91 символов
    invalid_input_msg: db "Invalid input", 0Ah, 0 ; 14 символов
    info: db "Choise the comand:", 0Ah, "0 - to close program", 0Ah, "1 - insert node to tree", 0Ah, "2 - delete node from tree", 0Ah, "3 - output tree", 0Ah, "4 - back up tree", 0Ah, "5 - balance the tree", 0Ah  ; 144 символов
    insert_value: db "Enter to insert value node in range from -9 223 372 036 854 775 808 until 9 223 372 036 854 775 807: ", 0   ; 101 символов
    delete_value: db "Enter to delete value node in range from -9 223 372 036 854 775 808 until 9 223 372 036 854 775 807: ", 0   ; 101 символов
    myTree: db "Root -> Left -> Right:", 0Ah ; 23 символов
    full_tree_msg: db "TREE IS FULL !!!", 0Ah ; 17 символов
    not_found_msg: db "Element not found :(", 0Ah ; 21 символ
    balance_success_msg: db "Success balance tree", 0Ah ; 21 символов

    file_name: db "tree.txt", 0 ; 8 символов 
    file_error_msg: db "Error to open file", 0Ah ; 22 символа
    file_invalid_msg: db "Incorrect data in file", 0Ah ; 23 символа
    file_success_msg: db "Success back up of tree", 0Ah ; 25 символов
    size_file dq 0
    key_file dq 0

    separator: db 0Ah

    node_count dw 0            ; Число узлов дерева
    last_node dq 0             ; Адрес последней node
    parent_in_delete dq 0

    max_nodes_count equ 256

    node_count_balance dw 0    ; будем сохранять кол-во элементов перед балансировкой
    average_index dq 0

section .bss
    treeNode resb 6144         ; Выделение 6144 байт для 256 узлов
    bufferFile resb 5120

    sortedArray resq 256       ; Массив для хранения элементов (для балансировки)
    arrayIndex resq 0          ; Индекс текущего элемента (для балансировки)
    
section .text         

;;;-------------------------------MAIN PROGRAM-----------------------------------------------Starting

bst:                     
    mov rax, 1             ; 1 - номер системного вызова функции write
    mov rdi, 1             ; 1 - дескриптор файла стандартного вызова stdout
    mov rsi, msRoot        ; адрес строки для вывода
    mov rdx, 91            ; количество байтов
    syscall

    mov rax, 0             ; sys_read
    mov rdi, 0             ; stdin
    mov rsi, bufferInput
    mov rdx, 21
    syscall

    mov rsi, bufferInput
    call validate_input
    cmp bl, 2
    je invalid_input_root

    mov [rel treeNode], rax
    inc byte [rel node_count]  ; Увеличиваем счетчик узлов

    lea rax, [rel treeNode] ; mov rax, treeNode
    mov [rel last_node], rax

continue_execution:
    mov ax, [rel node_count]
    cmp ax, 0
    je root_insert

    mov rax, 1             
    mov rdi, 1             
    mov rsi, info        
    mov rdx, 144   
    syscall

    mov rax, 0             ; sys_read
    mov rdi, 0             ; stdin
    mov rsi, bufferInput
    mov rdx, 21
    syscall

    xor ax, ax
    mov al, [rel bufferInput]
    add al, [rel bufferInput + 1] ; num + 0Ah

    cmp al, 58
    jb .invalid_input_main
    cmp al, 63
    ja .invalid_input_main

    cmp al, 59
    je insertCall

    cmp al, 60
    je deleteCall

    cmp al, 61
    je start_pre_order_traversal

    cmp al, 62
    je restore_tree_from_file

    cmp al, 63
    je balance_tree

    mov rax, 60               ; Завершение программы
    syscall

.invalid_input_main:
    mov rax, 1
    mov rdi, 1
    mov rsi, invalid_input_msg
    mov rdx, 14
    syscall

    jmp continue_execution

;;;-------------------------------MAIN PROGRAM-----------------------------------------------Ending

;;;-------------------------------ROOT INSERT-----------------------------------------------Strting

root_insert:
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, msRoot
    mov rdx, 91
    syscall

    mov rax, 0             ; sys_read
    mov rdi, 0             ; stdin
    mov rsi, bufferInput
    mov rdx, 21
    syscall

    mov rsi, bufferInput
    call validate_input
    cmp bl, 2
    je invalid_input_node

    mov [rel treeNode], rax

    inc byte [rel node_count]  ; Увеличиваем счетчик узлов

    mov rcx, [rel last_node]
    add rcx, 24              ; Узлы занимают 24 байт
    mov [rel last_node], rcx

    jmp continue_execution

;;;-------------------------------ROOT INSERT-----------------------------------------------Ending

;;;-------------------------------CHECK INPUT------------------------------------------------Starting

invalid_input_node:
    mov rax, 1
    mov rdi, 1
    mov rsi, invalid_input_msg
    mov rdx, 14
    syscall

    jmp insertCall

invalid_input_root:
    mov rax, 1
    mov rdi, 1
    mov rsi, invalid_input_msg
    mov rdx, 14
    syscall

    jmp bst

validate_input:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx   
    xor rdx, rdx
    mov bl, 1                 ; Флаг для отрицательного числа

    lodsb                     ; Загрузить первый символ из буфера в al
    cmp al, '-'               ; Проверка на знак минуса
    je check_digits           ; Если минус, перейти к проверке цифр
    cmp al, 0Ah
    je not_number
    mov bl, 0                 ; Флаг для положительного числа

back_to_digits:
    cmp al, 0Ah               ; Проверка на конец строки (новая строка)
    je end_check              ; Конец строки
    cmp al, '0'               ; Проверка, является ли это цифрой
    jb not_number
    cmp al, '9'
    ja not_number

    cmp bl, 1
    je convertNEG_to_number

convert_to_number:
    imul rdx, rdx, 10         ; Умножение текущего числа на 10
    jo overflow
    sub al, '0'               ; Конвертация ASCII в число
    add rdx, rax              ; Добавление к общему
    jo overflow

    jmp check_digits

convertNEG_to_number:
    imul rdx, rdx, 10
    jo overflow
    sub al, '0'
    sub rdx, rax
    jo overflow

    jmp check_digits

check_digits:
    lodsb                     ; Загрузить следующий символ из буфера

    jmp back_to_digits        ; Повторить проверку цифр

not_number:
    mov bl, 2                 ; Индикатор некорректного ввода

    ret

end_check:
    mov rax, rdx              ; Результат в rax

    ret

overflow:
    mov bl, 2                 ; Индикатор переполнения

    ret

;;;-------------------------------CHECK INPUT------------------------------------------------Ending

;;;-------------------------------INSERT NODE------------------------------------------------Starting

insertCall:
    call start_pre_order_traversal_to_file

    mov ax, [rel node_count]
    cmp ax, max_nodes_count
    je .full_tree

    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, insert_value
    mov rdx, 101
    syscall

    mov rax, 0             ; sys_read
    mov rdi, 0             ; stdin
    mov rsi, bufferInput
    mov rdx, 21
    syscall

    mov rsi, bufferInput
    call validate_input
    cmp bl, 2
    je invalid_input_node

    call insert

    jmp continue_execution

.full_tree:
    mov rax, 1
    mov rdi, 1
    mov rsi, full_tree_msg
    mov rdx, 17
    syscall

    jmp continue_execution

insert:
    lea rsi, [rel treeNode] ;mov rsi, treeNode , Адрес node(начинаем с корня)
    call insert_recursive

    ret

insert_recursive:
    mov rdx, rsi
    lea rcx, [rel treeNode]
    cmp rsi, rcx
    jne no_root_insert
    mov rdx, [rsi]
    mov rdi, [rsi]

    cmp rax, rdi
    jl .left_branch            ; Идем влево, если значение < текущего узла
    jge .right_branch          ; Идем вправо, если значение > текущего узла

    ret

.left_branch:
    ; Переход к левому потомку
    add rsi, 8                    ; Левый потомок

    mov rdi, [rsi]

    test rdi, rdi
    jz create_node

    call insert_recursive

    ret

.right_branch:
    ; Переход к правому потомку
    add rsi, 16                    ; Правый потомок

    mov rdi, [rsi]

    test rdi, rdi
    jz create_node

    call insert_recursive

    ret

no_root_insert:
    mov rdi, [rdx] ; значение ноды
    test rdi, rdi
    jz create_node_no_root

    mov rbx, [rdi]
    mov rdi, rbx

    cmp rax, rdi
    jl .left_branch_no_root            ; Идем влево, если значение < текущего узла
    jge .right_branch_no_root          ; Идем вправо, если значение > текущего узла

.left_branch_no_root:
    mov rdx, [rsi] ; aдрес ноды
    add rdx, 8
    mov rsi, rdx

    call no_root_insert

    ret

.right_branch_no_root:
    mov rdx, [rsi] ; aдрес ноды
    add rdx, 16
    mov rsi, rdx

    call no_root_insert

    ret

create_node:
    mov rcx, [rel last_node]
    add rcx, 24              ; Узлы занимают 24 байт
    mov [rel last_node], rcx

    mov [rcx], rax           ; Сохраняем значение
    mov [rsi], rcx           

    inc byte [rel node_count]    ; Увеличиваем счетчик узлов

    ret

create_node_no_root:
    mov rcx, [rel last_node]
    add rcx, 24              ; Узлы занимают 24 байт
    mov [rel last_node], rcx

    mov [rcx], rax           ; Сохраняем значение
    mov [rdx], rcx           

    inc byte [rel node_count]    ; Увеличиваем счетчик узлов

    ret

;;;-------------------------------INSERT NODE------------------------------------------------Ending

;;;-------------------------------OUTPUT TREE------------------------------------------------Starting

start_pre_order_traversal:
    mov rax, 1
    mov rdi, 1
    mov rsi, myTree
    mov rdx, 23
    syscall

    xor rdi, rdi
    lea rdi, [rel treeNode]

    call .pre_order_traversal

    jmp continue_execution    

.pre_order_traversal:
    test rdi, rdi
    jz done_pre_order_traversal

    ; Обрабатываем текущий узел
    mov rax, [rdi]
    call print_node_value

    ; Обходим левое поддерево
    add rdi, 8
    mov rsi, [rdi]
    sub rdi, 8

    push rdi

    test rsi, rsi
    jz .skip_left

    mov rdi, rsi
    call .pre_order_traversal

.skip_left:
    pop rdi
    ; Обходим правое поддерево
    mov rsi, [rdi + 16]
    test rsi, rsi
    jz done_pre_order_traversal
    mov rdi, rsi
    call .pre_order_traversal

done_pre_order_traversal:

    ret

print_node_value:
    mov r8, rdi

    ; Преобразуем число в строку
    mov rsi, bufferInt         ; Указатель на буфер
    call int_to_string

    ; Выводим строку на экран
    mov rdx, rax            ; Длина строки
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    syscall

    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, separator
    mov rdx, 1
    syscall

    mov rdi, r8

    ret

int_to_string:
    ; Преобразует число в строку
    ; Вход: rax = значение
    ; Выход: rsi = строка, rax = длина строки

    xor rbx, rbx            ; по умолчанию число положительное
    mov rcx, 10             ; Основание системы счисления
    lea rdi, [rsi + 20]     ; Конец буфера
    mov byte [rdi], 0       ; Null-terminator

    test rax, rax
    jns .convert_loop

    neg rax
    mov bl, '-'

.convert_loop:
    dec rdi
    xor rdx, rdx
    div rcx
    add dl, '0'
    mov [rdi], dl
    test rax, rax
    jnz .convert_loop

    ; Если число было отрицательным, добавляем минус
    cmp bl, '-'
    jne .done
    dec rdi
    mov [rdi], bl

.done:
    add rsi, 21
    mov rax, rsi
    sub rsi, 21
    sub rax, rdi            ; Длина строки
    mov rsi, rdi            ; Указатель на начало строки

    ret

;;;-------------------------------OUTPUT TREE------------------------------------------------Ending

;;;-------------------------------DELETE NODE------------------------------------------------Starting

deleteCall:
    call start_pre_order_traversal_to_file

    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, delete_value   ; Reuse the prompt message
    mov rdx, 101
    syscall

    mov rax, 0              ; sys_read
    mov rdi, 0              ; stdin
    mov rsi, bufferInput
    mov rdx, 21
    syscall

    lea rsi, [rel bufferInput]
    call validate_input
    cmp bl, 2
    je invalid_input_node

    mov rsi, 0
    mov [rel parent_in_delete], rsi
    lea rsi, [rel treeNode]   ; Адрес node(начинаем с корня)
    call delete_node

    jmp continue_execution

delete_node:
    test rsi, rsi
    jz return_node           ; If root is null, return

    mov rdx, [rsi]           ; Load current node's key
    mov rdi, rdx

    lea r8, [rel treeNode]
    cmp rsi, r8
    jne no_root

later_delete: ; rdi - num, rdx - adress num
    cmp rax, rdi
    jl .go_left                 ; If key < current node, go left
    jg .go_right                ; If key > current node, go right

    mov rdi, [rsi + 8]          ; левый сын
    mov rbx, [rsi + 16]          ; правый сын

    mov rdx, [rel parent_in_delete] ; отец

    test rdi, rdi
    jz .no_left_child
    test rbx, rbx
    jz .no_right_child

    ; Node has two children
    ; Find the max in the left subtree
    call find_maximum

    jmp .offset_ref

.no_left_child:
    ; rsi адрес удаляемого
    ; rdi адрес левого сына удаляемого (нет)
    ; rbx адрес правого сына удаляемого
    ; rdx адрес отца

    cmp rdx, 0
    je .no_left_child_is_root

    cmp rbx, 0
    je .no_right_child_in_deleted_node

    cmp [rdx + 8], rsi
    je .parent_get_left_child

    mov rax, rbx
    mov [rdx + 16], rax

    jmp .offset_ref

.no_left_child_is_root:
    jmp .offset_ref 

.parent_get_left_child:
    mov rax, rbx
    mov [rdx + 8], rax 

    jmp .offset_ref

.no_right_child_in_deleted_node:
    mov rax, [rsi]
    cmp rax, [rdx]
    jge .right_child_null

    xor rax, rax
    mov [rdx + 8], rax

    jmp .offset_ref

.right_child_null:
    xor rax, rax
    mov [rdx + 16], rax

    jmp .offset_ref

.no_right_child:
    ; rsi адрес удаляемого
    ; rdi адрес левого сына удаляемого
    ; rbx адрес правого сына удаляемого (нет)
    ; rdx адрес отца

    cmp rdx, 0
    je .no_right_child_is_root
    
    cmp rdi, 0
    je .no_right_child_in_deleted_node

    cmp [rdx + 8], rsi
    je .parent_get_left_child_no_R

    mov rax, rdi
    mov [rdx + 16], rax 

    jmp .offset_ref

.no_right_child_is_root:
    jmp .offset_ref

.parent_get_left_child_no_R:
    mov rax, rdi
    mov [rdx + 8], rax 

    jmp .offset_ref

.offset:
    mov rax, [rsi + 24]
    mov [rsi], rax

    mov rax, [rsi + 32]
    mov [rsi + 8], rax

    mov rax, [rsi + 40]
    mov [rsi + 16], rax

    add rsi, 24
    mov rax, [rsi]
    add rax, [rsi + 8]
    add rax, [rsi + 16]

    cmp rax, 0
    jne .offset

    dec byte [rel node_count]

    mov rax, [rel last_node]
    sub rax, 24
    mov [rel last_node], rax

    ret

.offset_ref:
    lea rax, [rel treeNode]
    lea rbx, [rel treeNode]
    add rax, 8
    add rbx, 16

.later_offset_ref:
    mov rcx, [rax - 8] 
    add rcx, [rax]
    add rcx, [rbx] 
    cmp rcx, 0
    je .offset

    mov rcx, [rax]
    cmp rcx, rsi
    jg .offset_left_node

.right:
    mov rcx, [rbx]
    cmp rcx, rsi
    jg .offset_right_node

    jmp .next_ref

.offset_left_node:
    mov rcx, [rax]
    sub rcx, 24
    mov [rax], rcx

    jmp .right

.offset_right_node:
    mov rcx, [rbx]
    sub rcx, 24
    mov [rbx], rcx

.next_ref:
    add rax, 24
    add rbx, 24

    jmp .later_offset_ref

.go_left:
    mov rcx, 1 ; флаг поворота налево

    add rsi, 8
    call delete_node
    sub rsi, 8

    ret

.go_right:
    mov rcx, 0 ; флаг поворота направо

    add rsi, 16
    call delete_node
    sub rsi, 16

    ret

return_node:

    ret

no_root:
    cmp rdx, 0
    je .notFound
    mov rdi, [rdx]

    cmp rcx, 1
    je .turnedLeft

    sub rsi, 16
    mov [rel parent_in_delete], rsi
    add rsi, 16

.later_no_root:
    mov rbx, [rsi] 
    mov rsi, rbx    

    jmp later_delete

.turnedLeft:
    sub rsi, 8
    mov [rel parent_in_delete], rsi
    add rsi, 8

    jmp .later_no_root

.notFound:
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, not_found_msg
    mov rdx, 21
    syscall

    jmp return_node

find_maximum:
    ; rsi адрес удаляемого
    ; rdi адрес левого сына удаляемого
    ; rbx адрес правого сына удаляемого
    ; rdx адрес отца

    mov rcx, rsi ; удаляемая node

    ;find num to replace deleted node
    mov rdx, rsi
    mov rsi, [rdx + 8]
    mov rdi, [rsi + 8]
    mov rbx, [rsi + 16]

    cmp rbx, 0
    je .found_maximum_to_left

.find_in_left_subtree:
    cmp rbx, 0
    je .found_maximum

    mov rdx, rsi
    mov rsi, [rsi + 16]
    mov rdi, [rsi + 8]
    mov rbx, [rsi + 16]
    jmp .find_in_left_subtree

.found_maximum_to_left:
    mov rax, [rsi]
    mov [rcx], rax

    mov [rdx + 8], rdi

    ret

.found_maximum:
    mov rax, [rsi]
    mov [rcx], rax

    mov [rdx + 16], rdi

    ret

;;;-------------------------------DELETE NODE------------------------------------------------Ending

;;;-------------------------------SERIALIZE TREE TO FILE----------------------------------------Starting

start_pre_order_traversal_to_file:
    mov rax, 2                 ; sys_open
    lea rdi, [rel file_name]   ; имя файла
    mov rsi, 01101o            ; O_WRONLY | O_CREAT | O_TRUNC
    mov rdx, 0644o             ; Права доступа
    syscall
    cmp rax, 0
    js file_error             ; Проверка на ошибку открытия
    mov [rel key_file], rax   ; Сохранить дескриптор файла

    xor rdi, rdi
    lea rdi, [rel treeNode]

    call .pre_order_traversal_to_file

    ; Закрыть файл
    mov rax, 3                 ; sys_close
    mov rdi, [rel key_file]        ; дескриптор файла
    syscall

    ret

.pre_order_traversal_to_file:
    test rdi, rdi
    jz done_pre_order_traversal_to_file

    ; Обработка текущего узла
    mov rax, [rdi]
    call write_node_value_to_file

    ; Обход левого поддерева
    add rdi, 8
    mov rsi, [rdi]
    sub rdi, 8

    push rdi

    test rsi, rsi
    jz .skip_left_to_file
    mov rdi, rsi
    call .pre_order_traversal_to_file

.skip_left_to_file:
    pop rdi
    ; Обход правого поддерева
    mov rsi, [rdi + 16]
    test rsi, rsi
    jz done_pre_order_traversal_to_file
    mov rdi, rsi
    call .pre_order_traversal_to_file

done_pre_order_traversal_to_file:

    ret

write_node_value_to_file:
    mov r8, rdi

    ; Преобразование числа в строку
    mov rsi, bufferInt         ; Указатель на буфер
    call int_to_string

    ; Запись строки в файл
    dec rax            
    mov rdx, rax               ; Длина строки
    mov rax, 1                 ; sys_write
    mov rdi, [rel key_file]    ; дескриптор файла
    syscall

    ; Запись новой строки
    mov rax, 1                 ; sys_write
    mov rdi, [rel key_file]    ; дескриптор файла
    mov rsi, separator
    mov rdx, 1
    syscall

    mov rdi, r8

    ret

;;;-------------------------------SERIALIZE TREE TO FILE----------------------------------------Ending

;;;-------------------------------RESTORE TREE----------------------------------------------Starting

restore_tree_from_file:
    mov rax, 2                 ; sys_open
    lea rdi, [rel file_name]   ; имя файла
    mov rsi, 0                 ; O_RDONLY
    syscall

    cmp rax, 0
    js file_error              ; Проверка на ошибку открытия

    mov [rel key_file], rax    ; Сохранить дескриптор файлa

    call clear_tree            ; Очистить текущее дерево

    mov rax, 0                 ; sys_read
    mov rdi, [rel key_file]    ; дескриптор файла
    lea rsi, [rel bufferFile]  ; буфер для чтения
    mov rdx, 5120              ; размер буфера
    syscall

    cmp rax, 0
    jle .close_file            ; Ошибка или конец файла

    mov [rel size_file], rax   ; Размер прочитанного
    lea r8, [rel bufferFile]   ; Начало буфера

.process_lines:
    ; Найти конец строки
    mov rax, [rel size_file]
    cmp rax, 0
    jle .close_file 

    mov rcx, 21
    mov rdi, r8
    mov rax, 0Ah
    repne scasb
    mov rsi, r8
    mov r8, rdi
    sub rdi, rsi               ; Длина строки
    jz .close_file             ; Конец файла

    ;уменьшим переменную размера на прочитаную часть
    mov rax, [rel size_file]  
    sub rax, rdi
    mov [rel size_file], rax

    ; Преобразование строки в число
    push rcx                   ; Сохранить оставшийся размер

    ; rsi - адрес источника
    ; rdi - адрес назначения (bufferFileNum)
    ; rcx - количество байт для копирования
    mov rcx, rdi
    lea rdi, [rel bufferFileNum]  ; Установить адрес назначения
    rep movsb                   ; Копировать байты

    mov rsi, bufferFileNum 
    call validate_input
    pop rcx                    ; Восстановить оставшийся размер
    cmp bl, 2
    je .invalid                ; Неверные данные в файле

    mov bx, [rel node_count]
    cmp bx, 0
    je .root_insert_from_file

    call insert_value_from_file ; Вставить значение в дерево

    jmp .process_lines

.invalid:
    mov rax, 3                 ; sys_close
    mov rdi, [rel key_file]    ; дескриптор файла
    syscall

    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, file_invalid_msg
    mov rdx, 23
    syscall

    jmp continue_execution


.root_insert_from_file:
    push rcx

    mov [rel treeNode], rax

    inc byte [rel node_count]  ; Увеличиваем счетчик узлов
    lea rcx, [rel treeNode]
    mov [rel last_node], rcx

    pop rcx

    jmp .process_lines

.close_file:
    mov rax, 3                 ; sys_close
    mov rdi, [rel key_file]    ; дескриптор файла
    syscall

    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, file_success_msg
    mov rdx, 25
    syscall

    jmp continue_execution

file_error:
    ; Обработка ошибки открытия файла
    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, file_error_msg
    mov rdx, 19
    syscall

    jmp continue_execution

clear_tree:
    ; Установим все байты в treeNode в 0
    xor rax, rax
    lea rdi, [rel treeNode]  ; Адрес начала дерева
    mov ax, [rel node_count] 
    imul rax, 24             ; Очистить кол-во узлов * 24 байта 
    mov rcx, rax
    xor rax, rax            

.clear_loop:
    mov [rdi], rax           ; Запись 0 в текущий узел
    add rdi, 8               ; Переход к следующему полю (по 8 байт)
    sub rcx, 8               ; Уменьшение счетчика
    jnz .clear_loop          ; Повтор, пока не очистим все

    ; Сбросить счетчик узлов
    xor rax, rax
    mov [rel node_count], ax   
    mov [rel last_node], rax

    ret

insert_value_from_file:
    mov rax, rdx               ; Значение для вставки
    call insert              

    ret

;;;-------------------------------RESTORE TREE----------------------------------------------Ending

;;;-------------------------------SORT ARRAY------------------------------------------------Starting

bubble_sort:
    lea rdi, [rel sortedArray]
    xor rcx, rcx
    mov cx, [rel node_count]

    push rbx
    push rsi
    push rdi

    mov rsi, rdi     ; rsi указывает на начало массива
    mov rbx, rcx     ; rbx хранит размер массива

    dec rbx          ; уменьшаем на 1, т.к. нам нужно n-1 сравнений
outer_loop:
    xor rdi, rdi     ; rdi будет использоваться как индекс

inner_loop:
    mov rax, [rsi + rdi * 8]     ; загружаем элемент array[i]
    mov rdx, [rsi + rdi * 8 + 8] ; загружаем элемент array[i+1]

    cmp rax, rdx
    jle no_swap

    ; меняем местами array[i] и array[i+1]
    mov [rsi + rdi * 8], rdx
    mov [rsi + rdi * 8 + 8], rax

no_swap:
    inc rdi
    cmp rdi, rbx
    jl inner_loop

    dec rbx
    jnz outer_loop

    pop rdi
    pop rsi
    pop rbx

    ret

;;;-------------------------------SORT ARRAY------------------------------------------------Ending

;;;-------------------------------BALANCE TREE----------------------------------------------Starting

balance_tree:
    xor rax, rax
    mov [rel arrayIndex], rax
    mov ax, [rel node_count]
    cmp ax, 2
    jle .end_balance_tree

    lea r8, [rel sortedArray]
    xor rcx, rcx

    ; 1. Обход и сбор элементов
    call .in_order_traversal_to_array

    ; 2. Сортировка элементов
    call bubble_sort
     
    ; 3. Очистка дерева
    call clear_tree 

    ; 4. Перестроение дерева
    lea rsi, [rel sortedArray]
    mov cx, [rel node_count_balance]
    call rebuild_tree

.end_balance_tree:

    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, balance_success_msg
    mov rdx, 21
    syscall

    jmp continue_execution

.in_order_traversal_to_array:
    xor rsi, rsi               ; Начальный адрес (корень дерева)
    lea rsi, [rel treeNode]    ; Начинаем с корня

    call .in_order_linear

    ret

.in_order_linear:
    mov cx, [rel node_count]  ; Общее количество узлов
    mov [rel node_count_balance], cx
    test rcx, rcx
    jz .done

.loop:
    test rsi, rsi
    jz .done

    ; Сохраняем текущее значение узла
    mov rax, [rsi]             ; Значение текущего узла
    mov rbx, [rel arrayIndex]

    ;sortedArray + rbx * 8	
    shl rbx, 3
    add rbx, r8
    mov [rbx], rax
    sub rbx, r8
    shr rbx, 3

    inc qword [rel arrayIndex]

    ; Переходим к следующему узлу
    add rsi, 24                ; Переход к следующему узлу

    loop .loop

.done:

    ret

rebuild_tree:
    ; Параметры: rsi = начало массива
    ;            rcx = количество элементов
    test rcx, rcx
    jz .done

    ; Вычисление среднего индекса
    mov rbx, rcx
    shr rbx, 1  ; rbx = rcx / 2

    mov ax, [rel node_count]
    cmp ax, 0
    je .root_insert_rebuild

    push rsi
    push rbx
    push rcx

    mov rax, [rsi + rbx * 8]  ; Загрузка среднего элемента
    call insert

    pop rcx
    pop rbx
    pop rsi

.left_right_branch:
    ; Рекурсивное построение левого поддерева
    push rbx
    push rsi
    push rcx
    mov rcx, rbx  ; Левая половина
    call rebuild_tree

    ; Рекурсивное построение правого поддерева
    pop rcx
    pop rsi
    pop rbx
    lea rsi, [rsi + (rbx + 1) * 8]
    sub rcx, rbx
    dec rcx
    test rcx, rcx
    jle .done
    call rebuild_tree

    ret

.root_insert_rebuild:
    mov rax, [rsi + rbx * 8] ; Установка среднего элемента как нового корня
    
    push rcx

    mov [rel treeNode], rax

    inc byte [rel node_count]  ; Увеличиваем счетчик узлов
    lea rcx, [rel treeNode]
    mov [rel last_node], rcx

    pop rcx

    jmp .left_right_branch

.done:

    ret

;;;-------------------------------BALANCE TREE----------------------------------------------Ending