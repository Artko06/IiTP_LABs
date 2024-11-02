.model small   
.data
    message1 db "qwertyuiop[]\asdfghjkl;'zxcvbnm,./1234567890!@#`~:<>#@|+=_-",0Dh,0Ah,'$'  
    message2 db "ASSembler",0Dh,0Ah,'$'     
    message3 db "It's very hard program",0Dh,0Ah,'$' 
    
.code 

print macro out_str
 mov AH,9
 mov DX,offset out_str
 int 21h
endm

start: 
       print message1   
       print message2
       print message3  
        
       end start