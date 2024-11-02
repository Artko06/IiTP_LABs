 LDX  #$8200
 LDY  #$82FF
 LDAB #$FF

 LDAA #$1A ; Выберем рандомное число для записи в А

FillMemory:
 JSR   Random     ; Генерация случайного числа в регистре A
 STAA  0,X 
 INX 
 DECB
 BNE   FillMemory ; Повторять, пока не заполнены все ячейки(пока B!=0)

SortLoop:
 LDX #$8200
InnerLoop:   
 LDAA  0,X
 CPX   #$82FF ; Проверить достигнут ли последний элемент
 BEQ   Done
 
 LDAB  1,X
 CBA 
 BGE   NoSwap      

 ; Произвести swap если следующий элемент больше текущего
 LDAA  0,X
 LDAB  1,X
 STAA  1,X
 STAB  0,X

NoSwap:
 INX
 BRA   InnerLoop ; Повторить внутренний цикл

Done:
 DEY            ; Уменьшить конечный адрес для следующей итерации
 CPY   #$8200   ; Проверить, завершена ли сортировка
 BHI   SortLoop ; Если нет, повторить сортировку
 JMP   End

Random:
    ROLA
    RTS 

End:    