.model small

.stack 100h

.data
cmd_size db ?
maxCMDSize equ 127
cmd_text db maxCMDSize + 2 dup(0)
folderPath db maxCMDSize + 2 dup(0)

DTAsize equ 2Ch     
DTAblock db DTAsize dup(0)

startText db "Program is started", '$'
endText db "Program is ended", '$'
reallocErrorMessage db "Bad relocation memory. Error code: ", '$'
runEXEErrorText db "Error running other program. Error code: ", '$'
badFolderErrorText db "Open directory error", '$'

                                       
EPBstruct 	dw 0                        
			dw offset line,0
			dw 005Ch, 0, 006Ch, 0		
line db 125
	 db " /?"
line_text db 122 dup(?)

EPBlen dw $ - EPBstruct

extensionEXEFile db "*.exe", 0

DataLength = $ - cmd_size
            
.code

newline MACRO
	push ax
	push dx
	
	mov dl, 10
	mov ah, 02h
	int 21h

	mov dl, 13
	mov ah, 02h
	int 21h

	pop dx
	pop ax
ENDM

println MACRO info
	push ax
	push dx

	mov ah, 09h
	mov dx, offset info
	int 21h

	newline

	pop dx
	pop ax
ENDM

printErrorCode MACRO   
	add al, '0'
	mov dl, al
	mov ah, 06h
	int 21h

	newline
ENDM

main:	                                                              
	mov ah, 4Ah                                                  
	mov bx, ((CodeLength / 16) + 1) + ((DataLength / 16) + 1) + 32	
	int 21h

	jnc startOfMainProgram                                        
                                                                   
	println reallocErrorMessage

	printErrorCode

	mov ax, 1

	jmp endMain

startOfMainProgram:
	mov ax, @data
	mov es, ax

	xor ch, ch
	mov cl, ds:[80h]			
	mov cmd_size, cl 		   
	mov bh, cl
	dec bh
	mov si, 82h                 
	mov di, offset cmd_text
	rep movsb

	mov ds, ax                

	println startText
    mov cmd_size, bh
   
	call parseCMD
	
	mov ah, 3Bh                           
    mov dx, offset folderPath
    int 21h
    jc openDirectoryError                 

	call findFirstFile
	cmp ax, 0
	jne endMain				   

	call runEXE
	cmp ax, 0
	jne endMain				    

runFile:
	call findNextFile
	cmp ax, 0
	jne endMain				    

	call runEXE
	cmp ax, 0
	jne endMain				   

	jmp runFile

openDirectoryError:
	println badFolderErrorText
	
endMain:
	println endText

	mov ah, 4Ch             
	int 21h

parseCMD PROC
	push bx
    push cx
    push dx
    
	mov si, offset cmd_text
    mov di, offset folderPath
    mov cl, cmd_size
    xor ch,ch
    rep movsb
    
    pop dx
    pop cx
	pop bx
	ret	
parseCMD ENDP

runEXE PROC
	push bx
	push dx

	mov ax, 4B00h				  
	mov bx, offset EPBstruct        
	mov dx, offset DTAblock + 1Eh
	int 21h
	
	jnc runEXEAllGood

	println runEXEErrorText
	printErrorCode

	mov ax, 1

	jmp runEXEEnd

runEXEAllGood:
	mov ax, 0

runEXEEnd:
	pop dx
	pop bx
	ret
runEXE ENDP

installDTA PROC
	mov ah,1Ah                    
    mov dx, offset DTAblock
    int 21h
    ret
installDTA ENDP

findFirstFile PROC
	call installDTA

    mov ah,4Eh                      
    xor cx,cx               		 
    mov dx, offset extensionEXEFile 
    int 21h

	jnc findFirstFileAllGood

	mov ax, 1

	jmp findFirstFileEnd

findFirstFileAllGood:
	mov ax, 0

findFirstFileEnd:

	ret
findFirstFile ENDP

findNextFile PROC
	call installDTA

	mov ah,4Fh                    
    mov dx, offset DTAblock       
    int 21h
    
	jnc findNextFileAllGood

	mov ax, 1

	jmp findNextFileEnd

findNextFileAllGood:
	mov ax, 0

findNextFileEnd:

	ret
findNextFile ENDP

CodeLength = $ - main

end main
