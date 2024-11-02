.model small
.stack 100h

.data 
ID_file1 dw 0h
ID_file2 dw 0h
maxSizeCmd equ 127
file1 db maxSizeCmd dup(?)
file2 db maxSizeCmd dup(?)  
maxSize equ 1024
buffer1 db maxSize dup(?)
buffer2 db maxSize dup(?)
info_cmd db 'You must input to cmd through the space name File or Path to File', 0Dh, 0Ah, '$'
msg_equal db 'Files are equal', 0Dh, 0Ah, '$'
msg_not_equal db 'Files are not equal', 0Dh, 0Ah, '$' 
msgError db 'Incorrect input to cmd', 0Dh, 0Ah, '$'

msgFoundFile db "File isn't found", 0Dh, 0Ah, '$'
msgFoundPath db "Path isn't found", 0Dh, 0Ah, '$'
msgManyFiles db "A lot of files opening", 0Dh, 0Ah, '$'
msgDenied db "Access denied", 0Dh, 0Ah, '$' 
msgMode db "Invalid access mode", 0Dh, 0Ah, '$'

.code 

output MACRO str 
    mov ah, 09h
    mov dx,offset str
    int 21h
endm

nameFile proc
    
    mov si, 0081h 
    cmp es:[si], 0Dh
    je errorName
    
    mov si, 0082h   ; first symbol 
    mov di, offset file1

firstFile:     
    mov al, es:[si]          
    
    cmp al, 0Dh     ; symbol str end
    je errorName                      
    
    cmp al, ' '
    je startSecondFile   
    
    mov [di], al 
    inc si
    inc di 
    jmp firstFile 
    
startSecondFile:
    mov di, offset file2
    
secondFile:
    mov al, es:[si]
    cmp al, 0Dh
    je endName
    
    mov [di], al
    inc si
    inc di
    jmp secondFile
    
endName:
    ret
    
nameFile endp


main proc
    mov ax, @data
    mov ds, ax   
    
    output info_cmd
    
    call nameFile 

    mov ah, 3Dh
    lea dx, file1
    mov al, 0
    int 21h
    jc errorOpen
    mov ID_file1, ax
    
    mov ah, 3Dh
    lea dx, file2
    mov al, 0
    int 21h
    jc errorOpen
    mov ID_file2, ax

compare_files:
    mov ah, 3Fh
    lea dx, buffer1
    mov bx, ID_file1
    mov cx, maxSize
    int 21h
    jc error
    mov si, ax

    mov ah, 3Fh
    lea dx, buffer2
    mov bx, ID_file2
    mov cx, maxSize
    int 21h
    jc error
    mov di, ax

    cmp si, 0
    je end_of_file
    cmp di, 0
    je end_of_file
    cmp si, di
    jne not_equal

    mov cx, si
    cmp si, di
    je compare_buffers
    mov cx, di

compare_buffers:
    xor si, si
    xor di, di 
    
compare_loop:
    mov al, buffer1[si]
    mov bl, buffer2[di]
    cmp al, bl
    jne not_equal
    inc si
    inc di
    loop compare_loop

    jmp compare_files

end_of_file:
    cmp si, 0
    jne not_equal
    cmp di, 0
    jne not_equal
    
    output msg_equal
    jmp done

not_equal:
    output msg_not_equal

done:
    mov ah, 3Eh
    int 21h
    mov ah, 3Eh
    mov bx, cx
    int 21h

    mov ah, 4Ch
    int 21h

error:  
    output msg_not_equal
    jmp done

errorName:  
    output msgError
    jmp done

errorOpen:
    cmp al, 02h 
    je FoundFile
    cmp al, 03h
    je FoundPath
    cmp al, 04h
    je ManyFiles
    cmp al, 05h
    je Denied
    cmp al, 0Ch
    je Mode
    jmp done 
    
FoundFile:
    output msgFoundFile
    jmp done
    
FoundPath:
    output msgFoundPath
    jmp done 
    
ManyFiles:
    output msgManyFiles
    jmp done
     
Denied:
    output msgDenied
    jmp done  

Mode:
    output msgMode
    jmp done

main endp
end main
