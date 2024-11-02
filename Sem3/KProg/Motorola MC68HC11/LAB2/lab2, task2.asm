 LDX  #$8200
 LDY  #$82FF
 LDAB #$FF

 LDAA #$1A ; ������� ��������� ����� ��� ������ � �

FillMemory:
 JSR   Random     ; ��������� ���������� ����� � �������� A
 STAA  0,X 
 INX 
 DECB
 BNE   FillMemory ; ���������, ���� �� ��������� ��� ������(���� B!=0)

SortLoop:
 LDX #$8200
InnerLoop:   
 LDAA  0,X
 CPX   #$82FF ; ��������� ��������� �� ��������� �������
 BEQ   Done
 
 LDAB  1,X
 CBA 
 BGE   NoSwap      

 ; ���������� swap ���� ��������� ������� ������ ��������
 LDAA  0,X
 LDAB  1,X
 STAA  1,X
 STAB  0,X

NoSwap:
 INX
 BRA   InnerLoop ; ��������� ���������� ����

Done:
 DEY            ; ��������� �������� ����� ��� ��������� ��������
 CPY   #$8200   ; ���������, ��������� �� ����������
 BHI   SortLoop ; ���� ���, ��������� ����������
 JMP   End

Random:
    ROLA
    RTS 

End:    