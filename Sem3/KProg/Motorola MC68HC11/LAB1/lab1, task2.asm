 LDX #$ffff
 LDY #$ffff
 
 CLR $8000 ; очистка А и B, освобождение 8 байт под X и Y и для результатов промежуточного вычисления
 CLR $8001
 CLR $8002
 CLR $8003
 CLR $8004
 CLR $8005
 CLR $8006
 CLR $8007
 CLR $8008
 CLR $8009
 CLR $800A
 CLR $800B
 CLRA
 CLRB

 XGDX
 STD $8000
 XGDY
 STD $8002

 ; (2^16 * AX * AY) + (2^8 * AX * BY) + (2^8 * AY * BX) + (BX * BY)

 LDAA $8000 ; выполним произведение AX * AY
 LDAB $8002
 MUL
 STD  $8004

 LDAA $8000 ; выполним произведение AX * BY
 LDAB $8003
 MUL
 STD  $8006

 LDAA $8002 ; выполним произведение AY * BX
 LDAB $8001
 MUL
 STD $8008


 LDAA $8001 ; выполним произведение BX * BY
 LDAB $8003
 MUL
 STD  $800A

 CLR $8100 ; освободим память под ответ
 CLR $8101
 CLR $8102
 CLR $8103
 
 STAB $8103 ; первый байт заплним ответом
 
 LDD  $8004 ; будет прибавлять к старшему, чтобы избежать переполнения
 XGDX 
 LDAB $8006
 ABX
 LDAB $8008
 ABX 
      
 CLRA	
 CLRB
 XGDX
 STD  $8004 ; сохраним эти два байта, они ещё понодобятся
 CLRA
 CLRB

 LDAB $8007
 XGDX
 LDAB $8009
 ABX
 LDAB $800A
 ABX

 CLRA
 CLRB
 XGDX 
 STAB $8102 ; сохраним 3-й байт
 CLRB

 TAB ; найдём 1 и 2 байт используя $8004
 CLRA
 XGDX
 
 LDD $8004
 XGDX
 ABX
 XGDX
 STD $8100 ; сохраним 1 и 2 байт, ответ находится в $8100-8103 