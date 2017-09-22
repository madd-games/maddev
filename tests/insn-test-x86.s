bits 16						; MODE16
bits 32						; MODE32
bits 64						; MODE64

aaa						; MODE16 MODE32
aad						; MODE16 MODE32
aad 0x56					; MODE16 MODE32
aam						; MODE16 MODE32
aam 0x56					; MODE16 MODE32
aas						; MODE16 MODE32
adc al, 0x56					; MODE16 MODE32 MODE64
adc ax, 0x567					; MODE16 MODE32 MODE64
adc eax, 0x567					; MODE16 MODE32 MODE64
adc BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
adc WORD PTR test, 0x567			; MODE16 MODE32 MODE64
adc DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
adc BYTE PTR test, bl				; MODE16 MODE32 MODE64
adc WORD PTR test, bx				; MODE16 MODE32 MODE64
adc DWORD PTR test, ebx				; MODE16 MODE32 MODE64
adc QWORD PTR test, rbx				; MODE64
adc cl, BYTE PTR test				; MODE16 MODE32 MODE64
adc cx, WORD PTR test				; MODE16 MODE32 MODE64
adc ecx, DWORD PTR test				; MODE16 MODE32 MODE64
adc rcx, QWORD PTR test				; MODE64
adcx eax, DWORD PTR test			; MODE16 MODE32 MODE64
adcx rax, QWORD PTR test			; MODE64
add al, 0x56					; MODE16 MODE32 MODE64
add ax, 0x567					; MODE16 MODE32 MODE64
add eax, 0x567					; MODE16 MODE32 MODE64
add BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
add WORD PTR test, 0x567			; MODE16 MODE32 MODE64
add DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
add BYTE PTR test, bl				; MODE16 MODE32 MODE64
add WORD PTR test, bx				; MODE16 MODE32 MODE64
add DWORD PTR test, ebx				; MODE16 MODE32 MODE64
add QWORD PTR test, rbx				; MODE64
add cl, BYTE PTR test				; MODE16 MODE32 MODE64
add cx, WORD PTR test				; MODE16 MODE32 MODE64
add ecx, DWORD PTR test				; MODE16 MODE32 MODE64
add rcx, QWORD PTR test				; MODE64
addpd xmm1, xmm2				; MODE16 MODE32 MODE64
addpd xmm3, XMMWORD PTR test			; MODE32 MODE64
addps xmm5, xmm2				; MODE16 MODE32 MODE64
addsd xmm6, xmm4				; MODE16 MODE32 MODE64
addss xmm0, xmm2				; MODE16 MODE32 MODE64
addsubpd xmm1, xmm3				; MODE16 MODE32 MODE64
addsubps xmm2, xmm4				; MODE16 MODE32 MODE64
adox ecx, ebx					; MODE16 MODE32 MODE64
aesdec xmm1, xmm5				; MODE16 MODE32 MODE64
aesdeclast xmm5, xmm4				; MODE16 MODE32 MODE64
aesenc xmm6, xmm7				; MODE16 MODE32 MODE64
aesenclast xmm5, xmm1				; MODE16 MODE32 MODE64
aesimc xmm0, xmm2				; MODE16 MODE32 MODE64
; TODO: aeskeygenassist
and al, 0x67					; MODE16 MODE32 MODE64
and DWORD PTR test, 0xdead			; MODE16 MODE32 MODE64
and eax, DWORD PTR test				; MODE16 MODE32 MODE64
; TODO: andn
andpd xmm1, xmm2				; MODE16 MODE32 MODE64
andps xmm2, xmm5				; MODE16 MODE32 MODE64
andnpd xmm3, xmm2				; MODE16 MODE32 MODE64
arpl ax, bx					; MODE16 MODE32
; TODO: blendpd
; TODO: bextr
; TODO: blendps
blendvpd xmm1, xmm5				; MODE16 MODE32 MODE64
; TODO: vblendvpd
blendvps xmm1, xmm5				; MODE16 MODE32 MODE64
; TODO: vlendvps
; TODO: blsi
; TODO: blsmk
; TODO: blsr
; TODO: bndcl
; TODO: bndcu
; TODO: bndcn
; TODO: bndldx
; TODO: bndmk
; TODO: bndmov
; TODO: bndstx
bound eax, DWORD PTR test			; MODE16 MODE32
bsf eax, DWORD PTR test				; MODE16 MODE32 MODE64
bsr eax, DWORD PTR test				; MODE16 MODE32 MODE64
bswap rdx					; MODE16 MODE32 MODE64
bt DWORD PTR test, eax				; MODE16 MODE32 MODE64
bt al, 6					; MODE16 MODE32 MODE64
bt ax, 7					; MODE16 MODE32 MODE64
bt eax, 7					; MODE16 MODE32 MODE64
bt rax, 8					; MODE64
btc DWORD PTR test, eax				; MODE16 MODE32 MODE64
btc eax, 6					; MODE16 MODE32 MODE64
btr DWORD PTR test, eax				; MODE16 MODE32 MODE64
btr eax, 5					; MODE16 MODE32 MODE64
bts DWORD PTR test, eax				; MODE16 MODE32 MODE64
bts eax, 5					; MODE16 MODE32 MODE64
call 0x08:456					; MODE16 MODE32
call DWORD OFFSET test				; MODE16 MODE32 MODE64
call WORD PTR test				; MODE16 MODE32
call DWORD PTR test				; MODE16 MODE32
call QWORD PTR test				; MODE64
callf DWORD PTR test				; MODE16 MODE32 MODE64
cbw						; MODE16 MODE32 MODE64
cwde						; MODE16 MODE32 MODE64
cdqe						; MODE64
clac						; MODE16 MODE32 MODE64
clc						; MODE16 MODE32 MODE64
cld						; MODE16 MODE32 MODE64
clflush BYTE PTR test				; MODE16 MODE32 MODE64
clflush WORD PTR test				; MODE16 MODE32 MODE64
clflush DWORD PTR test				; MODE16 MODE32 MODE64
clflush QWORD PTR test				; MODE64
clflushopt BYTE PTR test			; MODE16 MODE32 MODE64
clflushopt WORD PTR test			; MODE16 MODE32 MODE64
clflushopt DWORD PTR test			; MODE16 MODE32 MODE64
clflushopt QWORD PTR test			; MODE64
cli						; MODE16 MODE32 MODE64
clts						; MODE16 MODE32 MODE64
clwb BYTE PTR test				; MODE16 MODE32 MODE64
clwb WORD PTR test				; MODE16 MODE32 MODE64
clwb DWORD PTR test				; MODE16 MODE32 MODE64
clwb QWORD PTR test				; MODE64
cmc						; MODE16 MODE32 MODE64
cmova ax, WORD PTR test				; MODE16 MODE32 MODE64
cmova eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmova rax, QWORD PTR test			; MODE64
cmovae ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovae eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovae rax, QWORD PTR test			; MODE64
cmovb ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovb eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovb rax, QWORD PTR test			; MODE64
cmovbe ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovbe eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovbe rax, QWORD PTR test			; MODE64
cmovc ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovc eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovc rax, QWORD PTR test			; MODE64
cmove ax, WORD PTR test				; MODE16 MODE32 MODE64
cmove eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmove rax, QWORD PTR test			; MODE64
cmovg ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovg eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovg rax, QWORD PTR test			; MODE64
cmovge ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovge eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovge rax, QWORD PTR test			; MODE64
cmovl ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovl eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovl rax, QWORD PTR test			; MODE64
cmovle ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovle eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovle rax, QWORD PTR test			; MODE64
cmovna ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovna eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovna rax, QWORD PTR test			; MODE64
cmovnae ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnae eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnae rax, QWORD PTR test			; MODE64
cmovnb ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnb eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnb rax, QWORD PTR test			; MODE64
cmovnbe ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnbe eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnbe rax, QWORD PTR test			; MODE64
cmovnc ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnc eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnc rax, QWORD PTR test			; MODE64
cmovne ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovne eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovne rax, QWORD PTR test			; MODE64
cmovng ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovng eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovng rax, QWORD PTR test			; MODE64
cmovnge ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnge eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnge rax, QWORD PTR test			; MODE64
cmovnl ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnl eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnl rax, QWORD PTR test			; MODE64
cmovnle ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnle eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnle rax, QWORD PTR test			; MODE64
cmovno ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovno eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovno rax, QWORD PTR test			; MODE64
cmovnp ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnp eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnp rax, QWORD PTR test			; MODE64
cmovns ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovns eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovns rax, QWORD PTR test			; MODE64
cmovnz ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovnz eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovnz rax, QWORD PTR test			; MODE64
cmovo ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovo eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovo rax, QWORD PTR test			; MODE64
cmovp ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovp eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovp rax, QWORD PTR test			; MODE64
cmovpe ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovpe eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovpe rax, QWORD PTR test			; MODE64
cmovpo ax, WORD PTR test			; MODE16 MODE32 MODE64
cmovpo eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovpo rax, QWORD PTR test			; MODE64
cmovs ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovs eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovs rax, QWORD PTR test			; MODE64
cmovz ax, WORD PTR test				; MODE16 MODE32 MODE64
cmovz eax, DWORD PTR test			; MODE16 MODE32 MODE64
cmovz rax, QWORD PTR test			; MODE64
cmp al, 0x56					; MODE16 MODE32 MODE64
cmp ax, 0x567					; MODE16 MODE32 MODE64
cmp eax, 0x567					; MODE16 MODE32 MODE64
cmp rax, 0x567					; MODE64
cmp BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
cmp WORD PTR test, 0x567			; MODE16 MODE32 MODE64
cmp DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
cmp QWORD PTR test, 0x567			; MODE64
cmp BYTE PTR test, al				; MODE16 MODE32 MODE64
cmp WORD PTR test, ax				; MODE16 MODE32 MODE64
cmp DWORD PTR test, eax				; MODE16 MODE32 MODE64
cmp QWORD PTR test, rax				; MODE64
cmp al, BYTE PTR test				; MODE16 MODE32 MODE64
cmp ax, WORD PTR test				; MODE16 MODE32 MODE64
cmp eax, DWORD PTR test				; MODE16 MODE32 MODE64
cmp rax, QWORD PTR test				; MODE64
; TODO: cmppd
; TODO: cmpps
cmps						; MODE16 MODE32 MODE64
cmpsb						; MODE16 MODE32 MODE64
cmpsw						; MODE16 MODE32 MODE64
cmpsd						; MODE16 MODE32 MODE64
cmpsq						; MODE64
; TODO: cmpsd
; TODO: cmpss
cmpxchg BYTE PTR test, al			; MODE16 MODE32 MODE64
cmpxchg WORD PTR test, ax			; MODE16 MODE32 MODE64
cmpxchg DWORD PTR test, eax			; MODE16 MODE32 MODE64
cmpxchg QWORD PTR test, rax			; MODE64
cmpxchg8b PTR test				; MODE16 MODE32 MODE64
cmpxchg16b PTR test				; MODE64
comisd xmm1, XMMWORD PTR test			; MODE32 MODE64
cpuid						; MODE16 MODE32 MODE64
cvtdq2pd xmm2, XMMWORD PTR test			; MODE32 MODE64
cvtdq2ps xmm3, XMMWORD PTR test			; MODE32 MODE64
cvtpd2dq xmm4, XMMWORD PTR test			; MODE32 MODE64
cvtpd2pi mm3, XMMWORD PTR test			; MODE16 MODE32 MODE64
cvtpd2ps xmm5, XMMWORD PTR test			; MODE32 MODE64
cvtpi2pd xmm6, XMMWORD PTR test			; MODE32 MODE64
cvtpi2ps xmm7, XMMWORD PTR test			; MODE32 MODE64
cvtpi2ps xmm0, XMMWORD PTR test			; MODE32 MODE64
cvtps2dq xmm1, XMMWORD PTR test			; MODE32 MODE64
cvtps2pd xmm2, XMMWORD PTR test			; MODE32 MODE64
cvtps2pi mm5, XMMWORD PTR test			; MODE16 MODE32 MODE64
cvtsd2si xmm3, XMMWORD PTR test			; MODE32 MODE64
cvtsd2ss xmm4, XMMWORD PTR test			; MODE32 MODE64
cvtsi2sd xmm5, XMMWORD PTR test			; MODE32 MODE64
cvtsi2ss xmm6, XMMWORD PTR test			; MODE32 MODE64
cvtss2sd xmm7, XMMWORD PTR test			; MODE32 MODE64
cvtss2si xmm0, XMMWORD PTR test			; MODE32 MODE64
cvttpd2dq xmm1, XMMWORD PTR test		; MODE32 MODE64
cvttpd2pi mm4, XMMWORD PTR test			; MODE16 MODE32 MODE64
cvttps2dq xmm2, XMMWORD PTR test		; MODE32 MODE64
cvttps2pi mm2, XMMWORD PTR test			; MODE16 MODE32 MODE64
cvttsd2si xmm3, XMMWORD PTR test		; MODE32 MODE64
cvttss2si xmm4, XMMWORD PTR test		; MODE32 MODE64
cwd						; MODE16 MODE32 MODE64
cdq						; MODE16 MODE32 MODE64
cqo						; MODE64
daa						; MODE16 MODE32
das						; MODE16 MODE32
dec BYTE PTR test				; MODE16 MODE32 MODE64
dec WORD PTR test				; MODE16 MODE32 MODE64
dec DWORD PTR test				; MODE16 MODE32 MODE64
dec QWORD PTR test				; MODE64
dec al						; MODE16 MODE32
dec ax						; MODE16 MODE32
dec eax						; MODE16 MODE32
div BYTE PTR test				; MODE16 MODE32 MODE64
div WORD PTR test				; MODE16 MODE32 MODE64
div DWORD PTR test				; MODE16 MODE32 MODE64
div QWORD PTR test				; MODE64
divpd xmm5, XMMWORD PTR test			; MODE32 MODE64
divps xmm6, XMMWORD PTR test			; MODE32 MODE64
divsd xmm7, XMMWORD PTR test			; MODE32 MODE64
divss xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: dppd
; TODO: dpps
emms						; MODE16 MODE32 MODE64
; TODO: enter
; TODO: extractps
f2xm1						; MODE16 MODE32 MODE64
fabs						; MODE16 MODE32 MODE64
fadd FLOAT PTR test				; MODE16 MODE32 MODE64
fadd DOUBLE PTR test				; MODE16 MODE32 MODE64
fadd st0, st1					; MODE16 MODE32 MODE64
fadd st2, st0					; MODE16 MODE32 MODE64
faddp st3, st0					; MODE16 MODE32 MODE64
faddp						; MODE16 MODE32 MODE64
fiadd DWORD PTR test				; MODE16 MODE32 MODE64
fiadd WORD PTR test				; MODE16 MODE32 MODE64
; TODO: fbld
; TODO: fbstp
fchs						; MODE16 MODE32 MODE64
fclex						; MODE16 MODE32 MODE64
fnclex						; MODE16 MODE32 MODE64
fcmovb st0, st4					; MODE16 MODE32 MODE64
fcmove st0, st5					; MODE16 MODE32 MODE64
fcmovbe st0, st6				; MODE16 MODE32 MODE64
fcmovu st0, st7					; MODE16 MODE32 MODE64
fcmovnb st0, st1				; MODE16 MODE32 MODE64
fcmovne st0, st2				; MODE16 MODE32 MODE64
fcmovnbe st0, st3				; MODE16 MODE32 MODE64
fcmovnu st0, st4				; MODE16 MODE32 MODE64
fcmovb st0, st5					; MODE16 MODE32 MODE64
fcom FLOAT PTR test				; MODE16 MODE32 MODE64
fcom DOUBLE PTR test				; MODE16 MODE32 MODE64
fcom st6					; MODE16 MODE32 MODE64
fcom						; MODE16 MODE32 MODE64
fcomp FLOAT PTR test				; MODE16 MODE32 MODE64
fcomp DOUBLE PTR test				; MODE16 MODE32 MODE64
fcomp st7					; MODE16 MODE32 MODE64
fcomp						; MODE16 MODE32 MODE64
fcompp 						; MODE16 MODE32 MODE64
fcomi st0, st1					; MODE16 MODE32 MODE64
fcomip st0, st2					; MODE16 MODE32 MODE64
fucomi st0, st3					; MODE16 MODE32 MODE64
fucomip st0, st4				; MODE16 MODE32 MODE64
fcos						; MODE16 MODE32 MODE64
fdcestp						; MODE16 MODE32 MODE64
fdiv FLOAT PTR test				; MODE16 MODE32 MODE64
fdiv DOUBLE PTR test				; MODE16 MODE32 MODE64
fdiv st0, st5					; MODE16 MODE32 MODE64
fdiv st6, st0					; MODE16 MODE32 MODE64
fdivp st7, st0					; MODE16 MODE32 MODE64
fdivp						; MODE16 MODE32 MODE64
fidiv DWORD PTR test				; MODE16 MODE32 MODE64
fidiv WORD PTR test				; MODE16 MODE32 MODE64
fdivr FLOAT PTR test				; MODE16 MODE32 MODE64
fdivr DOUBLE PTR test				; MODE16 MODE32 MODE64
fdivr st0, st1					; MODE16 MODE32 MODE64
fdivr st2, st0					; MODE16 MODE32 MODE64
fdivrp st3, st0					; MODE16 MODE32 MODE64
fdivrp						; MODE16 MODE32 MODE64
fidivr DWORD PTR test				; MODE16 MODE32 MODE64
fidivr WORD PTR test				; MODE16 MODE32 MODE64
ffree						; MODE16 MODE32 MODE64
ficom WORD PTR test				; MODE16 MODE32 MODE64
ficom DWORD PTR test				; MODE16 MODE32 MODE64
ficomp WORD PTR test				; MODE16 MODE32 MODE64
ficomp DWORD PTR test				; MODE16 MODE32 MODE64
fild WORD PTR test				; MODE16 MODE32 MODE64
fild DWORD PTR test				; MODE16 MODE32 MODE64
fild QWORD PTR test				; MODE16 MODE32 MODE64
finit						; MODE16 MODE32 MODE64
fninit						; MODE16 MODE32 MODE64
fist WORD PTR test				; MODE16 MODE32 MODE64
fist DWORD PTR test				; MODE16 MODE32 MODE64
fistp WORD PTR test				; MODE16 MODE32 MODE64
fistp DWORD PTR test				; MODE16 MODE32 MODE64
fistp QWORD PTR test				; MODE16 MODE32 MODE64
fisttp WORD PTR test				; MODE16 MODE32 MODE64
fisttp DWORD PTR test				; MODE16 MODE32 MODE64
fisttp QWORD PTR test				; MODE16 MODE32 MODE64
fld FLOAT PTR test				; MODE16 MODE32 MODE64
fld DOUBLE PTR test				; MODE16 MODE32 MODE64
fld FPUWORD PTR test				; MODE16 MODE32 MODE64
fld st4						; MODE16 MODE32 MODE64
fld1						; MODE16 MODE32 MODE64
fldl2t						; MODE16 MODE32 MODE64
fldl2e						; MODE16 MODE32 MODE64
fldpi						; MODE16 MODE32 MODE64
fldlg2						; MODE16 MODE32 MODE64
fldln2						; MODE16 MODE32 MODE64
fldz						; MODE16 MODE32 MODE64
fldcw WORD PTR test				; MODE16 MODE32 MODE64
fldenv PTR [eax]				; MODE16 MODE32 MODE64
fmul FLOAT PTR test				; MODE16 MODE32 MODE64
fmul DOUBLE PTR test				; MODE16 MODE32 MODE64
fmul st0, st5					; MODE16 MODE32 MODE64
fmul st6, st0					; MODE16 MODE32 MODE64
fmulp st7, st0					; MODE16 MODE32 MODE64
fmulp						; MODE16 MODE32 MODE64
fimul DWORD PTR test				; MODE16 MODE32 MODE64
fimul WORD PTR test				; MODE16 MODE32 MODE64
fnop						; MODE16 MODE32 MODE64
fpatan						; MODE16 MODE32 MODE64
fprem						; MODE16 MODE32 MODE64
fprem1						; MODE16 MODE32 MODE64
fptan						; MODE16 MODE32 MODE64
frndint						; MODE16 MODE32 MODE64
frstor PTR [eax]				; MODE16 MODE32 MODE64
fsave PTR [eax]					; MODE16 MODE32 MODE64
fnsave PTR [eax]				; MODE16 MODE32 MODE64
fscale						; MODE16 MODE32 MODE64
fsin						; MODE16 MODE32 MODE64
fsincos						; MODE16 MODE32 MODE64
fsqrt						; MODE16 MODE32 MODE64
fst FLOAT PTR test				; MODE16 MODE32 MODE64
fst DOUBLE PTR test				; MODE16 MODE32 MODE64
fst st1						; MODE16 MODE32 MODE64
fstp FLOAT PTR test				; MODE16 MODE32 MODE64
fstp DOUBLE PTR test				; MODE16 MODE32 MODE64
fstp FPUWORD PTR test				; MODE16 MODE32 MODE64
fstp st2					; MODE16 MODE32 MODE64
fstcw WORD PTR test				; MODE16 MODE32 MODE64
fnstcw WORD PTR test				; MODE16 MODE32 MODE64
fstenv PTR [eax]				; MODE16 MODE32 MODE64
fnstenv PTR [eax]				; MODE16 MODE32 MODE64
fstsw WORD PTR test				; MODE16 MODE32 MODE64
fstsw ax					; MODE16 MODE32 MODE64
fnstsw WORD PTR test				; MODE16 MODE32 MODE64
fnstsw ax					; MODE16 MODE32 MODE64
fsub FLOAT PTR test				; MODE16 MODE32 MODE64
fsub DOUBLE PTR test				; MODE16 MODE32 MODE64
fsub st0, st3					; MODE16 MODE32 MODE64
fsub st4, st0					; MODE16 MODE32 MODE64
fsubp st5, st0					; MODE16 MODE32 MODE64
fsubp						; MODE16 MODE32 MODE64
fisub DWORD PTR test				; MODE16 MODE32 MODE64
fisub WORD PTR test				; MODE16 MODE32 MODE64
fsubr FLOAT PTR test				; MODE16 MODE32 MODE64
fsubr DOUBLE PTR test				; MODE16 MODE32 MODE64
fsubr st0, st6					; MODE16 MODE32 MODE64
fsubr st7, st0					; MODE16 MODE32 MODE64
fsubpr st1, st0					; MODE16 MODE32 MODE64
fsubpr						; MODE16 MODE32 MODE64
fisubr DWORD PTR test				; MODE16 MODE32 MODE64
fisubr WORD PTR test				; MODE16 MODE32 MODE64
ftst						; MODE16 MODE32 MODE64
fucom st2					; MODE16 MODE32 MODE64
fucom						; MODE16 MODE32 MODE64
fucomp st3					; MODE16 MODE32 MODE64
fucomp						; MODE16 MODE32 MODE64
fucompp						; MODE16 MODE32 MODE64
fxam						; MODE16 MODE32 MODE64
fxch st4					; MODE16 MODE32 MODE64
fxch						; MODE16 MODE32 MODE64
fxrstor PTR [eax]				; MODE16 MODE32 MODE64
fxsave PTR [eax]				; MODE16 MODE32 MODE64
fxtract						; MODE16 MODE32 MODE64
fyl2x						; MODE16 MODE32 MODE64
fyl2xp1						; MODE16 MODE32 MODE64
haddpd xmm1, XMMWORD PTR test			; MODE32 MODE64
haddps xmm2, XMMWORD PTR test			; MODE32 MODE64
hlt						; MODE16 MODE32 MODE64
hsubpd xmm3, XMMWORD PTR test			; MODE32 MODE64
hsubps xmm4, XMMWORD PTR test			; MODE32 MODE64
idiv BYTE PTR test				; MODE16 MODE32 MODE64
idiv WORD PTR test				; MODE16 MODE32 MODE64
idiv DWORD PTR test				; MODE16 MODE32 MODE64
idiv QWORD PTR test				; MODE64
imul BYTE PTR test				; MODE16 MODE32 MODE64
imul WORD PTR test				; MODE16 MODE32 MODE64
imul DWORD PTR test				; MODE16 MODE32 MODE64
imul QWORD PTR test				; MODE64
imul ax, WORD PTR test				; MODE16 MODE32 MODE64
imul eax, DWORD PTR test			; MODE16 MODE32 MODE64
imul rax, QWORD PTR test			; MODE64
imul ax, WORD PTR test, 0x56			; MODE16 MODE32 MODE64
imul eax, DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
imul rax, QWORD PTR test, 0x56			; MODE64
imul ax, WORD PTR test, 0x567			; MODE16 MODE32 MODE64
imul eax, DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
imul rax, QWORD PTR test, 0x567			; MODE64
in al, 0x56					; MODE16 MODE32 MODE64
in ax, 0x56					; MODE16 MODE32 MODE64
in eax, 0x56					; MODE16 MODE32 MODE64
in al, dx					; MODE16 MODE32 MODE64
in ax, dx					; MODE16 MODE32 MODE64
in eax, dx					; MODE16 MODE32 MODE64
inc BYTE PTR test				; MODE16 MODE32 MODE64
inc WORD PTR test				; MODE16 MODE32 MODE64
inc DWORD PTR test				; MODE16 MODE32 MODE64
inc QWORD PTR test				; MODE64
inc ax						; MODE16 MODE32
inc eax						; MODE16 MODE32
insb						; MODE16 MODE32 MODE64
insw						; MODE16 MODE32 MODE64
insd						; MODE16 MODE32 MODE64
; TODO: insertps
int 3						; MODE16 MODE32 MODE64
int 0x56					; MODE16 MODE32 MODE64
into						; MODE16 MODE32
invd						; MODE16 MODE32 MODE64
invplg PTR [eax]				; MODE16 MODE32 MODE64
invpcid	eax, DWORD PTR test			; MODE16 MODE32 MODE64
invpcid	rax, QWORD PTR test			; MODE64
iret						; MODE16 MODE32 MODE64
iretd						; MODE16 MODE32 MODE64
iretq						; MODE64
ja BYTE OFFSET test				; MODE16 MODE32 MODE64
ja WORD OFFSET test				; MODE16 MODE32 MODE64
ja DWORD OFFSET test				; MODE16 MODE32 MODE64
jae BYTE OFFSET test				; MODE16 MODE32 MODE64
jb BYTE OFFSET test				; MODE16 MODE32 MODE64
jbe BYTE OFFSET test				; MODE16 MODE32 MODE64
jc BYTE OFFSET test				; MODE16 MODE32 MODE64
jcxz BYTE OFFSET test				; MODE16 MODE32
jecxz BYTE OFFSET test				; MODE16 MODE32 MODE64
jrcxz BYTE OFFSET test				; MODE64
je BYTE OFFSET test				; MODE16 MODE32 MODE64
jg BYTE OFFSET test				; MODE16 MODE32 MODE64
jge BYTE OFFSET test				; MODE16 MODE32 MODE64
jl BYTE OFFSET test				; MODE16 MODE32 MODE64
jle BYTE OFFSET test				; MODE16 MODE32 MODE64
jna BYTE OFFSET test				; MODE16 MODE32 MODE64
jnae BYTE OFFSET test				; MODE16 MODE32 MODE64
jnb BYTE OFFSET test				; MODE16 MODE32 MODE64
jnbe BYTE OFFSET test				; MODE16 MODE32 MODE64
jnc BYTE OFFSET test				; MODE16 MODE32 MODE64
jne BYTE OFFSET test				; MODE16 MODE32 MODE64
jng BYTE OFFSET test				; MODE16 MODE32 MODE64
jnge BYTE OFFSET test				; MODE16 MODE32 MODE64
jnl BYTE OFFSET test				; MODE16 MODE32 MODE64
jnle BYTE OFFSET test				; MODE16 MODE32 MODE64
jno BYTE OFFSET test				; MODE16 MODE32 MODE64
jnp BYTE OFFSET test				; MODE16 MODE32 MODE64
jns BYTE OFFSET test				; MODE16 MODE32 MODE64
jnz BYTE OFFSET test				; MODE16 MODE32 MODE64
jo BYTE OFFSET test				; MODE16 MODE32 MODE64
jp BYTE OFFSET test				; MODE16 MODE32 MODE64
jpe BYTE OFFSET test				; MODE16 MODE32 MODE64
jpo BYTE OFFSET test				; MODE16 MODE32 MODE64
js BYTE OFFSET test				; MODE16 MODE32 MODE64
jz BYTE OFFSET test				; MODE16 MODE32 MODE64
jmp BYTE OFFSET test				; MODE16 MODE32 MODE64
jmp WORD OFFSET test				; MODE16 MODE32
jmp DWORD OFFSET test				; MODE16 MODE32 MODE64
jmp DWORD PTR test				; MODE16 MODE32
jmp QWORD PTR test				; MODE64
jmp 0x08:0x56					; MODE16 MODE32
jmpf DWORD PTR test				; MODE16 MODE32 MODE64
lahf						; MODE16 MODE32 MODE64
lar ax, bx					; MODE16 MODE32 MODE64
lar ax, WORD PTR test				; MODE16 MODE32 MODE64
lddqu xmm5, XMMWORD PTR test			; MODE32 MODE64
ldmxcsr DWORD PTR test				; MODE32 MODE64
lds ax, WORD PTR test				; MODE16 MODE32
lds eax, DWORD PTR test				; MODE16 MODE32
lss ax, WORD PTR test				; MODE16 MODE32 MODE64
lss eax, DWORD PTR test				; MODE16 MODE32 MODE64
lss rax, QWORD PTR test				; MODE64
les ax, WORD PTR test				; MODE16 MODE32
les eax, DWORD PTR test				; MODE16 MODE32
lfs ax, WORD PTR test				; MODE16 MODE32 MODE64
lfs eax, DWORD PTR test				; MODE16 MODE32 MODE64
lss rax, QWORD PTR test				; MODE64
lgs ax, WORD PTR test				; MODE16 MODE32 MODE64
lgs eax, DWORD PTR test				; MODE16 MODE32 MODE64
lgs rax, QWORD PTR test				; MODE64
lea ax, WORD PTR test				; MODE16 MODE32 MODE64
lea eax, DWORD PTR test				; MODE16 MODE32 MODE64
lea rax, QWORD PTR test				; MODE64
leave						; MODE16 MODE32 MODE64
lldt WORD PTR test				; MODE16 MODE32 MODE64
lmsw WORD PTR test				; MODE16 MODE32 MODE64
lock						; MODE16 MODE32 MODE64
lodsb						; MODE16 MODE32 MODE64
lodsw						; MODE16 MODE32 MODE64
lodsd						; MODE16 MODE32 MODE64
lodsq						; MODE64
loop BYTE OFFSET test				; MODE16 MODE32 MODE64
loope BYTE OFFSET test				; MODE16 MODE32 MODE64
loopne BYTE OFFSET test				; MODE16 MODE32 MODE64
lsl ax, WORD PTR test				; MODE16 MODE32 MODE64
lsl eax, DWORD PTR test				; MODE16 MODE32 MODE64
lsl rax, QWORD PTR test				; MODE64
ltr WORD PTR test				; MODE16 MODE32 MODE64
lzcnt bx, WORD PTR test				; MODE16 MODE32 MODE64
lzcnt ebx, DWORD PTR test			; MODE16 MODE32 MODE64
lzcnt rbx, QWORD PTR test			; MODE64
maskmovdqu xmm3, xmm2				; MODE32 MODE64
maskmovq mm1, mm5				; MODE16 MODE32 MODE64
maxpd xmm4, XMMWORD PTR test			; MODE32 MODE64
maxps xmm4, XMMWORD PTR test			; MODE32 MODE64
maxsd xmm4, XMMWORD PTR test			; MODE32 MODE64
maxss xmm4, XMMWORD PTR test			; MODE32 MODE64
mfence						; MODE16 MODE32 MODE64
minpd xmm4, XMMWORD PTR test			; MODE32 MODE64
minps xmm4, XMMWORD PTR test			; MODE32 MODE64
minsd xmm4, XMMWORD PTR test			; MODE32 MODE64
minss xmm4, XMMWORD PTR test			; MODE32 MODE64
monitor						; MODE16 MODE32 MODE64
mov al, BYTE PTR test				; MODE16 MODE32 MODE64
mov ax, WORD PTR test				; MODE16 MODE32 MODE64
mov eax, DWORD PTR test				; MODE16 MODE32 MODE64
mov rax, QWORD PTR test				; MODE64
mov BYTE PTR test, al				; MODE16 MODE32 MODE64
mov WORD PTR test, ax				; MODE16 MODE32 MODE64
mov DWORD PTR test, eax				; MODE16 MODE32 MODE64
mov QWORD PTR test, rax				; MODE64
mov WORD PTR test, cs				; MODE16 MODE32 MODE64
mov WORD PTR test, ds				; MODE16 MODE32 MODE64
mov WORD PTR test, es				; MODE16 MODE32 MODE64
mov WORD PTR test, fs				; MODE16 MODE32 MODE64
mov WORD PTR test, gs				; MODE16 MODE32 MODE64
mov WORD PTR test, ss				; MODE16 MODE32 MODE64
mov QWORD PTR test, ss				; MODE64
mov ss, ax					; MODE16 MODE32 MODE64
mov ss, rax					; MODE64
mov BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
mov WORD PTR test, 0x567			; MODE16 MODE32 MODE64
mov DWORD PTR test, 0x456			; MODE16 MODE32 MODE64
mov QWORD PTR test, 0x453			; MODE64
mov cr0, ax					; MODE16 MODE32 MODE64
mov ax, cr0					; MODE16 MODE32 MODE64
mov dr0, ax					; MODE16 MODE32 MODE64
mov ax, dr0					; MODE16 MODE32 MODE64
mov db0, ax					; MODE16 MODE32 MODE64
mov ax, db0					; MODE16 MODE32 MODE64
movapd xmm5, XMMWORD PTR test			; MODE32 MODE64
movapd XMMWORD PTR test, xmm5			; MODE32 MODE64
movaps xmm6, XMMWORD PTR test			; MODE32 MODE64
movaps XMMWORD PTR test, xmm6			; MODE32 MODE64
movbe ax, WORD PTR byte				; MODE16 MODE32 MODE64
movbe eax, DWORD PTR byte			; MODE16 MODE32 MODE64
movbe rax, QWORD PTR byte			; MODE64
movbe WORD PTR byte, ax				; MODE16 MODE32 MODE64
movbe DWORD PTR byte, eax			; MODE16 MODE32 MODE64
movbe QWORD PTR byte, rax			; MODE64
movd mm0, eax					; MODE32 MODE64
movq mm0, rax					; MODE64
movd eax, mm0					; MODE32 MODE64
movq rax, mm0					; MODE64
movd xmm0, eax					; MODE32 MODE64
movq xmm0, rax					; MODE64
movd eax, xmm0					; MODE32 MODE64
movq rax, xmm0					; MODE64
movddup xmm1, XMMWORD PTR test			; MODE32 MODE64
movdqa xmm2, XMMWORD PTR test			; MODE32 MODE64
movdqa XMMWORD PTR test, xmm2			; MODE32 MODE64
movdqu xmm2, XMMWORD PTR test			; MODE32 MODE64
movdqu XMMWORD PTR test, xmm2			; MODE32 MODE64
movdq2q mm0, xmm3				; MODE16 MODE32 MODE64
movhlps xmm4, XMMWORD PTR test			; MODE32 MODE64
movhpd xmm5, XMMWORD PTR test			; MODE32 MODE64
movhpd XMMWORD PTR test, xmm5			; MODE32 MODE64
movhps xmm6, XMMWORD PTR test			; MODE32 MODE64
movhps XMMWORD PTR test, xmm6			; MODE32 MODE64
movlhps xmm7, XMMWORD PTR test			; MODE32 MODE64
movlpd xmm0, XMMWORD PTR test			; MODE32 MODE64
movlpd XMMWORD PTR test, xmm0			; MODE32 MODE64
movlps xmm1, XMMWORD PTR test			; MODE32 MODE64
movlps XMMWORD PTR test, xmm1			; MODE32 MODE64
movmskpd eax, xmm0				; MODE32 MODE64
movmskpd rax, xmm11				; MODE32 MODE64
movmskps eax, xmm0				; MODE32 MODE64
movmskps rax, xmm11				; MODE32 MODE64
movntdqa xmm2, XMMWORD PTR test			; MODE32 MODE64
movntdq XMMWORD PTR test, xmm3			; MODE32 MODE64
movnti DWORD PTR test, eax			; MODE16 MODE32 MODE64
movnti QWORD PTR test, rax			; MODE64
movntpd XMMWORD PTR test, xmm4			; MODE32 MODE64
movntps XMMWORD PTR test, xmm5			; MODE32 MODE64
movntq QWORD PTR test, mm0			; MODE16 MODE32 MODE64
movq mm0, QWORD PTR test			; MODE32 MODE64
movq QWORD PTR test, mm0			; MODE32 MODE64
movq mm0, XMMWORD PTR test			; MODE32 MODE64
movq XMMWORD PTR test, mm0			; MODE32 MODE64
movq2dq xmm0, mm0				; MODE16 MODE32 MODE64
movsb						; MODE16 MODE32 MODE64
movsw						; MODE16 MODE32 MODE64
movsd						; MODE16 MODE32 MODE64
movsq						; MODE64
movsd xmm0, XMMWORD PTR test			; MODE32 MODE64
movsd XMMWORD PTR test, xmm0			; MODE32 MODE64
movshdup xmm0, XMMWORD PTR test			; MODE32 MODE64
movsldup xmm0, XMMWORD PTR test			; MODE32 MODE64
movss xmm0, XMMWORD PTR test			; MODE32 MODE64
movss XMMWORD PTR test, xmm0			; MODE32 MODE64
movsx ax, BYTE PTR test				; MODE16 MODE32 MODE64
movsx eax, BYTE PTR test			; MODE16 MODE32 MODE64
movsx rax, BYTE PTR test			; MODE64
movsx eax, WORD PTR test			; MODE16 MODE32 MODE64
movsx rax, WORD PTR test			; MODE64
movsxd rax, DWORD PTR test			; MODE64
movupd xmm0, XMMWORD PTR test			; MODE32 MODE64
movupd XMMWORD PTR test, xmm0			; MODE32 MODE64
movups xmm0, XMMWORD PTR test			; MODE32 MODE64
movups XMMWORD PTR test, xmm0			; MODE32 MODE64
movzx ax, BYTE PTR test				; MODE16 MODE32 MODE64
movzx eax, BYTE PTR test			; MODE16 MODE32 MODE64
movzx rax, BYTE PTR test			; MODE64
movzx eax, WORD PTR test			; MODE16 MODE32 MODE64
movzx rax, WORD PTR test			; MODE64
; TODO: mp2adbw
mul BYTE PTR test				; MODE16 MODE32 MODE64
mul WORD PTR test				; MODE16 MODE32 MODE64
mul DWORD PTR test				; MODE16 MODE32 MODE64
mul QWORD PTR test				; MODE64
mulpd xmm0, XMMWORD PTR test			; MODE32 MODE64
mulps xmm0, XMMWORD PTR test			; MODE32 MODE64
mulsd xmm0, XMMWORD PTR test			; MODE32 MODE64
mulss xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: mulx
mwait						; MODE16 MODE32 MODE64
