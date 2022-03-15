; file ps.as
.entry LIST
.extern W
kok→: add r3, LIST
LOOP: prn #+48k
macro mac1
 inc r6
 mov r3, W
endm
 lea STR, r6
 lea #6
 mac1
 sub r1,, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
0IllegalLabel: mov r3, r5
 sub LOOP[r8] ,r14
END: stop,
END: nonExistentOperation
STR: .string abcd"
LIST: .data 6, -9.5
 .data -100
.entry K
K: .data 31,
.data
.extern val1
.string
; testing:
lea x, y
lea #-6, y
sub #+5, r3

