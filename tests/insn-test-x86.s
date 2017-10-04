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
neg BYTE PTR test				; MODE16 MODE32 MODE64
neg r12l					; MODE64
neg WORD PTR test				; MODE16 MODE32 MODE64
neg DWORD PTR test				; MODE16 MODE32 MODE64
neg QWORD PTR test				; MODE64
nop						; MODE16 MODE32 MODE64
nop WORD PTR test				; MODE16 MODE32 MODE64
nop DWORD PTR test				; MODE16 MODE32 MODE64
not BYTE PTR test				; MODE16 MODE32 MODE64
not WORD PTR test				; MODE16 MODE32 MODE64
not DWORD PTR test				; MODE16 MODE32 MODE64
not QWORD PTR test				; MODE64
or ecx, 0x55					; MODE16 MODE32 MODE64
or al, 0x56					; MODE16 MODE32 MODE64
or ax, 0x567					; MODE16 MODE32 MODE64
or eax, 0x456					; MODE16 MODE32 MODE64
or rax, 0x456					; MODE64
or BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
or WORD PTR test, 0x567				; MODE16 MODE32 MODE64
or DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
or QWORD PTR test, 0x454			; MODE64
or BYTE PTR test, cl				; MODE16 MODE32 MODE64
or WORD PTR test, cx				; MODE16 MODE32 MODE64
or DWORD PTR test, ecx				; MODE16 MODE32 MODE64
or QWORD PTR test, rcx				; MODE64
or cl, BYTE PTR test				; MODE16 MODE32 MODE64
or cx, WORD PTR test				; MODE16 MODE32 MODE64
or ecx, DWORD PTR test				; MODE16 MODE32 MODE64
or rcx, QWORD PTR test				; MODE64
orpd xmm4, XMMWORD PTR test			; MODE16 MODE32 MODE64
orps xmm4, XMMWORD PTR test			; MODE16 MODE32 MODE64
out 0x56, al					; MODE16 MODE32 MODE64
out 0x56, ax					; MODE16 MODE32 MODE64
out 0x56, eax					; MODE16 MODE32 MODE64
out dx, al					; MODE16 MODE32 MODE64
out dx, ax					; MODE16 MODE32 MODE64
out dx, eax					; MODE16 MODE32 MODE64
outsb						; MODE16 MODE32 MODE64
outsw						; MODE16 MODE32 MODE64
outsd						; MODE16 MODE32 MODE64
pabsb mm0, QWORD PTR test			; MODE32 MODE64
pabsb xmm0, XMMWORD PTR test			; MODE32 MODE64
pabsw mm0, QWORD PTR test			; MODE32 MODE64
pabsw xmm0, XMMWORD PTR test			; MODE32 MODE64
pabsd mm0, QWORD PTR test			; MODE32 MODE64
pabsd xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PABSQ
packsswb mm0, QWORD PTR test			; MODE32 MODE64
packsswb xmm0, XMMWORD PTR test			; MODE32 MODE64
packssdw mm0, QWORD PTR test			; MODE32 MODE64
packssdw xmm0, XMMWORD PTR test			; MODE32 MODE64
packusdw xmm0, XMMWORD PTR test			; MODE32 MODE64
packuswb mm0, QWORD PTR test			; MODE32 MODE64
packuswb xmm0, XMMWORD PTR test			; MODE32 MODE64
paddb mm0, QWORD PTR test			; MODE32 MODE64
paddw mm0, QWORD PTR test			; MODE32 MODE64
paddb xmm0, XMMWORD PTR test			; MODE32 MODE64
paddw xmm0, XMMWORD PTR test			; MODE32 MODE64
paddd xmm0, XMMWORD PTR test			; MODE32 MODE64
paddq xmm0, XMMWORD PTR test			; MODE32 MODE64
paddsb mm0, QWORD PTR test			; MODE32 MODE64
paddsb xmm0, XMMWORD PTR test			; MODE32 MODE64
paddsw mm0, QWORD PTR test			; MODE32 MODE64
paddsw xmm0, XMMWORD PTR test			; MODE32 MODE64
paddusb mm0, QWORD PTR test			; MODE32 MODE64
paddusb xmm0, XMMWORD PTR test			; MODE32 MODE64
paddusw mm0, QWORD PTR test			; MODE32 MODE64
paddusw xmm0, XMMWORD PTR test			; MODE32 MODE64
;TODO palingr
pand mm0, QWORD PTR test			; MODE32 MODE64
pand xmm0, XMMWORD PTR test			; MODE32 MODE64
pandn mm0, QWORD PTR test			; MODE32 MODE64
pandn xmm0, XMMWORD PTR test			; MODE32 MODE64
pause						; MODE16 MODE32 MODE64
pavgb mm0, QWORD PTR test			; MODE32 MODE64
pavgb xmm0, XMMWORD PTR test			; MODE32 MODE64
pavgw mm0, QWORD PTR test			; MODE32 MODE64
pavgw xmm0, XMMWORD PTR test			; MODE32 MODE64
pblendvb xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: VPBLENDVB
; TODO: PBLENDW
; TODO: PCLMULQDQ
pcmpeqb mm0, QWORD PTR test			; MODE32 MODE64
pcmpeqb xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpeqw mm0, QWORD PTR test			; MODE32 MODE64
pcmpeqw xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpeqd mm0, QWORD PTR test			; MODE32 MODE64
pcmpeqd xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpeqq xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PCLMPESTRI
; TODO: PCLMPESTRM
pcmpgtb mm0, QWORD PTR test			; MODE32 MODE64
pcmpgtb xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpgtw mm0, QWORD PTR test			; MODE32 MODE64
pcmpgtw xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpgtd mm0, QWORD PTR test			; MODE32 MODE64
pcmpgtd xmm0, XMMWORD PTR test			; MODE32 MODE64
pcmpgtq xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PCMPISTRI
; TODO: PCMPISTRM
; TODO: PDEP
; TODO: PEXT
; TODO: PEXTRB/PEXTRD/PEXTRQ
; TODO: PEXTRW
phaddw mm0, QWORD PTR test			; MODE32 MODE64
phaddw xmm0, XMMWORD PTR test			; MODE32 MODE64
phaddd mm0, QWORD PTR test			; MODE32 MODE64
phaddd xmm0, XMMWORD PTR test			; MODE32 MODE64
phaddsw mm0, QWORD PTR test			; MODE32 MODE64
phaddsw xmm0, XMMWORD PTR test			; MODE32 MODE64
phminposuw xmm0, XMMWORD PTR test		; MODE32 MODE64
phsubw mm0, QWORD PTR test			; MODE32 MODE64
phsubw xmm0, XMMWORD PTR test			; MODE32 MODE64
phsubd mm0, QWORD PTR test			; MODE32 MODE64
phsubd xmm0, XMMWORD PTR test			; MODE32 MODE64
phsubsw mm0, QWORD PTR test			; MODE32 MODE64
phsubsw xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PINSRB/PINSRD/PINSRQ
; TODO: PINSRW
pmaddubsw mm0, QWORD PTR test			; MODE32 MODE64
pmaddubsw xmm0, XMMWORD PTR test		; MODE32 MODE64
pmaddwd mm0, QWORD PTR test			; MODE32 MODE64
pmaddwd xmm0, XMMWORD PTR test			; MODE32 MODE64
pmaxsw mm0, QWORD PTR test			; MODE32 MODE64
pmaxsb xmm0, XMMWORD PTR test			; MODE32 MODE64
pmaxsw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmaxsd xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PMAXSQ
pmaxub mm0, QWORD PTR test			; MODE32 MODE64
pmaxub xmm0, XMMWORD PTR test			; MODE32 MODE64
pmaxuw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmaxud xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PMAXUQ
pminsw mm0, QWORD PTR test			; MODE32 MODE64
pminsb xmm0, XMMWORD PTR test			; MODE32 MODE64
pminsw xmm0, XMMWORD PTR test			; MODE32 MODE64
pminsd xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PMINSQ
pminub mm0, QWORD PTR test			; MODE32 MODE64
pminub xmm0, XMMWORD PTR test			; MODE32 MODE64
pminuw xmm0, XMMWORD PTR test			; MODE32 MODE64
pminud xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PMINUQ
pmovmskb eax, mm1				; MODE32 MODE64
pmovmskb rax, xmm13				; MODE32 MODE64
pmovsxbw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovsxbd xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovsxbq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovsxwd xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovsxwq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovsxdq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxbw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxbd xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxbq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxwd xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxwq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmovzxdq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmuldq xmm0, XMMWORD PTR test			; MODE32 MODE64
pmulhrsw mm0, QWORD PTR test			; MODE32 MODE64
pmulhrsw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmulhuw mm0, QWORD PTR test			; MODE32 MODE64
pmulhuw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmulhw mm0, QWORD PTR test			; MODE32 MODE64
pmulhw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmulld xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PMULLQ
pmulw mm0, QWORD PTR test			; MODE32 MODE64
pmulw xmm0, XMMWORD PTR test			; MODE32 MODE64
pmuludq mm0, QWORD PTR test			; MODE32 MODE64
pmuludq xmm0, XMMWORD PTR test			; MODE32 MODE64
pop WORD PTR test				; MODE16 MODE32 MODE64
pop DWORD PTR test				; MODE16 MODE32
pop QWORD PTR test				; MODE64
pop ax 						; MODE16 MODE32 MODE64
pop eax 					; MODE16 MODE32
pop rax 					; MODE64
popa						; MODE16 MODE32
popad						; MODE16 MODE32
popcnt ax, WORD PTR test			; MODE16 MODE32 MODE64
popcnt eax, DWORD PTR test			; MODE16 MODE32 MODE64
popcnt rax, QWORD PTR test			; MODE64
popf						; MODE16 MODE32 MODE64
popfd						; MODE16 MODE32
popfq						; MODE64
por mm0, QWORD PTR test				; MODE32 MODE64
por xmm0, XMMWORD PTR test			; MODE32 MODE64
prefetch0 BYTE PTR test				; MODE16 MODE32 MODE64
prefetch1 BYTE PTR test				; MODE16 MODE32 MODE64
prefetch2 BYTE PTR test				; MODE16 MODE32 MODE64
prefetchnta BYTE PTR test			; MODE16 MODE32 MODE64
prefetchw BYTE PTR test				; MODE32 MODE64
prefetchwt1 BYTE PTR test			; MODE32 MODE64
psadbw mm0, QWORD PTR test			; MODE32 MODE64
psadbw xmm0, XMMWORD PTR test			; MODE32 MODE64
pshufb mm0, QWORD PTR test			; MODE32 MODE64
pshufb xmm0, XMMWORD PTR test			; MODE32 MODE64
; TODO: PSHUFD
; TODO: PSHUFHW
; TODO: PSHUFLW
; TODO: PSHUFW
psignb mm0, QWORD PTR test			; MODE32 MODE64
psignb xmm0, XMMWORD PTR test			; MODE32 MODE64
psignw mm0, QWORD PTR test			; MODE32 MODE64
psignw xmm0, XMMWORD PTR test			; MODE32 MODE64
psignd mm0, QWORD PTR test			; MODE32 MODE64
psignd xmm0, XMMWORD PTR test			; MODE32 MODE64
pslldq xmm0, 0x56				; MODE32 MODE64
psllw mm0, QWORD PTR test			; MODE32 MODE64
psllw xmm0, XMMWORD PTR test			; MODE32 MODE64
psllw mm0, 0x56					; MODE32 MODE64
psllw xmm0, 0x56				; MODE32 MODE64
pslld mm0, QWORD PTR test			; MODE32 MODE64
pslld xmm0, XMMWORD PTR test			; MODE32 MODE64
pslld mm0, 0x56					; MODE32 MODE64
pslld xmm0, 0x56				; MODE32 MODE64
psllq mm0, QWORD PTR test			; MODE32 MODE64
psllq xmm0, XMMWORD PTR test			; MODE32 MODE64
psllq mm0, 0x56					; MODE32 MODE64
psllq xmm0, 0x56				; MODE32 MODE64
psraw mm0, QWORD PTR test			; MODE32 MODE64
psraw xmm0, XMMWORD PTR test			; MODE32 MODE64
psraw mm0, 0x56					; MODE32 MODE64
psraw xmm0, 0x56				; MODE32 MODE64
psrad mm0, QWORD PTR test			; MODE32 MODE64
psrad xmm0, XMMWORD PTR test			; MODE32 MODE64
psrad mm0, 0x56					; MODE32 MODE64
psrad xmm0, 0x56				; MODE32 MODE64
psrldq xmm0, 0x56				; MODE32 MODE64
; TODO: PSRAQ
psrldq xmm0, 0x56				; MODE32 MODE64
psrlw mm0, QWORD PTR test			; MODE32 MODE64
psrlw xmm0, XMMWORD PTR test			; MODE32 MODE64
psrlw mm0, 0x56					; MODE32 MODE64
psrlw xmm0, 0x56				; MODE32 MODE64
psrld mm0, QWORD PTR test			; MODE32 MODE64
psrld xmm0, XMMWORD PTR test			; MODE32 MODE64
psrld mm0, 0x56					; MODE32 MODE64
psrld xmm0, 0x56				; MODE32 MODE64
psrlq mm0, QWORD PTR test			; MODE32 MODE64
psrlq xmm0, XMMWORD PTR test			; MODE32 MODE64
psrlq mm0, 0x56					; MODE32 MODE64
psllq xmm0, 0x56				; MODE32 MODE64
psubb mm0, QWORD PTR test			; MODE32 MODE64
psubb xmm0, XMMWORD PTR test			; MODE32 MODE64
psubw mm0, QWORD PTR test			; MODE32 MODE64
psubw xmm0, XMMWORD PTR test			; MODE32 MODE64
psubd mm0, QWORD PTR test			; MODE32 MODE64
psubd xmm0, XMMWORD PTR test			; MODE32 MODE64
psubq mm0, QWORD PTR test			; MODE32 MODE64
psubq xmm0, XMMWORD PTR test			; MODE32 MODE64
psubsb mm0, QWORD PTR test			; MODE32 MODE64
psubsb xmm0, XMMWORD PTR test			; MODE32 MODE64
psubsw mm0, QWORD PTR test			; MODE32 MODE64
psubsw xmm0, XMMWORD PTR test			; MODE32 MODE64
psubusb mm0, QWORD PTR test			; MODE32 MODE64
psubusb xmm0, XMMWORD PTR test			; MODE32 MODE64
psubusw mm0, QWORD PTR test			; MODE32 MODE64
psubusw xmm0, XMMWORD PTR test			; MODE32 MODE64
ptest xmm0, XMMWORD PTR test			; MODE32 MODE64
ptwrite DWORD PTR test				; MODE32 MODE64
ptwrite QWORD PTR test				; MODE64
punpckhbw mm0, QWORD PTR test			; MODE32 MODE64
punpckhbw xmm0, XMMWORD PTR test		; MODE32 MODE64
punpckhwd mm0, QWORD PTR test			; MODE32 MODE64
punpckhwd xmm0, XMMWORD PTR test		; MODE32 MODE64
punpckhdq mm0, QWORD PTR test			; MODE32 MODE64
punpckhdq xmm0, XMMWORD PTR test		; MODE32 MODE64
punpckhqdq xmm0, XMMWORD PTR test		; MODE32 MODE64
punpcklbw mm0, QWORD PTR test			; MODE32 MODE64
punpcklbw xmm0, XMMWORD PTR test		; MODE32 MODE64
punpcklwd mm0, QWORD PTR test			; MODE32 MODE64
punpcklwd xmm0, XMMWORD PTR test		; MODE32 MODE64
punpckldq mm0, QWORD PTR test			; MODE32 MODE64
punpckldq xmm0, XMMWORD PTR test		; MODE32 MODE64
punpcklqdq xmm0, XMMWORD PTR test		; MODE32 MODE64
push WORD PTR test				; MODE16 MODE32 MODE64
push DWORD PTR test				; MODE16 MODE32
push QWORD PTR test				; MODE64
push ax						; MODE16 MODE32 MODE64
push eax					; MODE16 MODE32
push rax					; MODE64
push 0x56					; MODE16 MODE32 MODE64
push 0x567					; MODE16 MODE32 MODE64
pusha						; MODE16 MODE32
pushad						; MODE16 MODE32
pushf						; MODE16 MODE32 MODE64
pushfd						; MODE16 MODE32
pushfq						; MODE64
pxor mm0, QWORD PTR test			; MODE32 MODE64
pxor xmm0, XMMWORD PTR test			; MODE32 MODE64
rcl BYTE PTR test, 1				; MODE16 MODE32 MODE64
rcl BYTE PTR test, cl				; MODE16 MODE32 MODE64
rcl BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
rcl WORD PTR test, 1				; MODE16 MODE32 MODE64
rcl WORD PTR test, cl				; MODE16 MODE32 MODE64
rcl WORD PTR test, 0x56				; MODE16 MODE32 MODE64
rcl DWORD PTR test, 1				; MODE16 MODE32 MODE64
rcl QWORD PTR test, 1				; MODE64
rcl DWORD PTR test, cl				; MODE16 MODE32 MODE64
rcl QWORD PTR test, cl				; MODE64
rcl DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
rcl QWORD PTR test, 0x56			; MODE64
rcl BYTE PTR test, 1				; MODE16 MODE32 MODE64
rcr BYTE PTR test, cl				; MODE16 MODE32 MODE64
rcr BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
rcr WORD PTR test, 1				; MODE16 MODE32 MODE64
rcr WORD PTR test, cl				; MODE16 MODE32 MODE64
rcr WORD PTR test, 0x56				; MODE16 MODE32 MODE64
rcr DWORD PTR test, 1				; MODE16 MODE32 MODE64
rcr QWORD PTR test, 1				; MODE64
rcr DWORD PTR test, cl				; MODE16 MODE32 MODE64
rcr QWORD PTR test, cl				; MODE64
rcr DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
rcr QWORD PTR test, 0x56			; MODE64
rol BYTE PTR test, 1				; MODE16 MODE32 MODE64
rol BYTE PTR test, cl				; MODE16 MODE32 MODE64
rol BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
rol WORD PTR test, 1				; MODE16 MODE32 MODE64
rol WORD PTR test, cl				; MODE16 MODE32 MODE64
rol WORD PTR test, 0x56				; MODE16 MODE32 MODE64
rol DWORD PTR test, 1				; MODE16 MODE32 MODE64
rol QWORD PTR test, 1				; MODE64
rol DWORD PTR test, cl				; MODE16 MODE32 MODE64
rol QWORD PTR test, cl				; MODE64
rol DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
rol QWORD PTR test, 0x56			; MODE64
ror BYTE PTR test, 1				; MODE16 MODE32 MODE64
ror BYTE PTR test, cl				; MODE16 MODE32 MODE64
ror BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
ror WORD PTR test, 1				; MODE16 MODE32 MODE64
ror WORD PTR test, cl				; MODE16 MODE32 MODE64
ror WORD PTR test, 0x56				; MODE16 MODE32 MODE64
ror DWORD PTR test, 1				; MODE16 MODE32 MODE64
ror QWORD PTR test, 1				; MODE64
ror DWORD PTR test, cl				; MODE16 MODE32 MODE64
ror QWORD PTR test, cl				; MODE64
ror DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
ror QWORD PTR test, 0x56			; MODE64
rcpps xmm0, XMMWORD PTR test			; MODE32 MODE64
rcpss xmm0, XMMWORD PTR test			; MODE32 MODE64
rdgsbase rax					; MODE64
rdgsbase rax					; MODE64
rdmsr 						; MODE16 MODE32 MODE64
rdpid eax					; MODE32
rdpid rax					; MODE64
rdpkru 						; MODE32 MODE64
rdpmc 						; MODE16 MODE32 MODE64
rdseed ax					; MODE32 MODE64
rdseed eax					; MODE32 MODE64
rdseed rax					; MODE64
rdseed ax					; MODE32 MODE64
rdseed eax					; MODE32 MODE64
rdseed rax					; MODE64
rdtsc 						; MODE16 MODE32 MODE64
rdtscp 						; MODE16 MODE32 MODE64
; TODO: REP/REPE/REPZ/REPNE/REPNZ
ret						; MODE16 MODE32 MODE64
ret 0x567					; MODE16 MODE32 MODE64
; TODO: ROUNDPD
; TODO: ROUNDPS
; TODO: ROUNDSD
; TODO: ROUNDSS
rsm						; MODE16 MODE32 MODE64
rsqrtps xmm0, XMMWORD PTR test			; MODE32 MODE64
rsqrtss xmm0, XMMWORD PTR test			; MODE32 MODE64
sahf						; MODE16 MODE32
sal BYTE PTR test, 1				; MODE16 MODE32 MODE64
sal BYTE PTR test, cl				; MODE16 MODE32 MODE64
sal BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
sal WORD PTR test, 1				; MODE16 MODE32 MODE64
sal WORD PTR test, cl				; MODE16 MODE32 MODE64
sal WORD PTR test, 0x56				; MODE16 MODE32 MODE64
sal DWORD PTR test, 1				; MODE16 MODE32 MODE64
sal QWORD PTR test, 1				; MODE64
sal DWORD PTR test, cl				; MODE16 MODE32 MODE64
sal QWORD PTR test, cl				; MODE64
sal DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
sal QWORD PTR test, 0x56			; MODE64
sal BYTE PTR test, 1				; MODE16 MODE32 MODE64
sar BYTE PTR test, cl				; MODE16 MODE32 MODE64
sar BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
sar WORD PTR test, 1				; MODE16 MODE32 MODE64
sar WORD PTR test, cl				; MODE16 MODE32 MODE64
sar WORD PTR test, 0x56				; MODE16 MODE32 MODE64
sar DWORD PTR test, 1				; MODE16 MODE32 MODE64
sar QWORD PTR test, 1				; MODE64
sar DWORD PTR test, cl				; MODE16 MODE32 MODE64
sar QWORD PTR test, cl				; MODE64
sar DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
sar QWORD PTR test, 0x56			; MODE64
shl BYTE PTR test, 1				; MODE16 MODE32 MODE64
shl BYTE PTR test, cl				; MODE16 MODE32 MODE64
shl BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
shl WORD PTR test, 1				; MODE16 MODE32 MODE64
shl WORD PTR test, cl				; MODE16 MODE32 MODE64
shl WORD PTR test, 0x56				; MODE16 MODE32 MODE64
shl DWORD PTR test, 1				; MODE16 MODE32 MODE64
shl QWORD PTR test, 1				; MODE64
shl DWORD PTR test, cl				; MODE16 MODE32 MODE64
shl QWORD PTR test, cl				; MODE64
shl DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
shl QWORD PTR test, 0x56			; MODE64
shr BYTE PTR test, 1				; MODE16 MODE32 MODE64
shr BYTE PTR test, cl				; MODE16 MODE32 MODE64
shr BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
shr WORD PTR test, 1				; MODE16 MODE32 MODE64
shr WORD PTR test, cl				; MODE16 MODE32 MODE64
shr WORD PTR test, 0x56				; MODE16 MODE32 MODE64
shr DWORD PTR test, 1				; MODE16 MODE32 MODE64
shr QWORD PTR test, 1				; MODE64
shr DWORD PTR test, cl				; MODE16 MODE32 MODE64
shr QWORD PTR test, cl				; MODE64
shr DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
shr QWORD PTR test, 0x56			; MODE64
; TODO: SARX/SHLX/SHRX
sbb al, 0x56					; MODE16 MODE32 MODE64
sbb ax, 0x567					; MODE16 MODE32 MODE64
sbb rax, 0x567					; MODE16 MODE32 MODE64
sbb eax, 0x567					; MODE64
sbb BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
sbb WORD PTR test, 0x567			; MODE16 MODE32 MODE64
sbb DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
sbb QWORD PTR test, 0x567			; MODE64
sbb WORD PTR test, 0x56				; MODE16 MODE32 MODE64
sbb DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
sbb QWORD PTR test, 0x56			; MODE64
sbb BYTE PTR test, al				; MODE16 MODE32 MODE64
sbb WORD PTR test, ax				; MODE16 MODE32 MODE64
sbb DWORD PTR test, eax				; MODE16 MODE32 MODE64
sbb QWORD PTR test, rax				; MODE64
sbb al, BYTE PTR test				; MODE16 MODE32 MODE64
sbb ax, WORD PTR test				; MODE16 MODE32 MODE64
sbb eax, DWORD PTR test				; MODE16 MODE32 MODE64
sbb rax, QWORD PTR test				; MODE64
scasb						; MODE16 MODE32 MODE64
scasw						; MODE16 MODE32 MODE64
scasd						; MODE16 MODE32 MODE64
scasq						; MODE64
seta BYTE PTR test				; MODE16 MODE32 MODE64
setae BYTE PTR test				; MODE16 MODE32 MODE64
setb BYTE PTR test				; MODE16 MODE32 MODE64
setbe BYTE PTR test				; MODE16 MODE32 MODE64
setc BYTE PTR test				; MODE16 MODE32 MODE64
sete BYTE PTR test				; MODE16 MODE32 MODE64
setg BYTE PTR test				; MODE16 MODE32 MODE64
setge BYTE PTR test				; MODE16 MODE32 MODE64
setl BYTE PTR test				; MODE16 MODE32 MODE64
setle BYTE PTR test				; MODE16 MODE32 MODE64
setna BYTE PTR test				; MODE16 MODE32 MODE64
setnae BYTE PTR test				; MODE16 MODE32 MODE64
setnb BYTE PTR test				; MODE16 MODE32 MODE64
setnbe BYTE PTR test				; MODE16 MODE32 MODE64
setnc BYTE PTR test				; MODE16 MODE32 MODE64
setne BYTE PTR test				; MODE16 MODE32 MODE64
setng BYTE PTR test				; MODE16 MODE32 MODE64
setnge BYTE PTR test				; MODE16 MODE32 MODE64
setnl BYTE PTR test				; MODE16 MODE32 MODE64
setnle BYTE PTR test				; MODE16 MODE32 MODE64
setno BYTE PTR test				; MODE16 MODE32 MODE64
setnp BYTE PTR test				; MODE16 MODE32 MODE64
setns BYTE PTR test				; MODE16 MODE32 MODE64
setnz BYTE PTR test				; MODE16 MODE32 MODE64
seto BYTE PTR test				; MODE16 MODE32 MODE64
setp BYTE PTR test				; MODE16 MODE32 MODE64
setpe BYTE PTR test				; MODE16 MODE32 MODE64
setpe BYTE PTR test				; MODE16 MODE32 MODE64
sets BYTE PTR test				; MODE16 MODE32 MODE64
setz BYTE PTR test				; MODE16 MODE32 MODE64
sfence						; MODE16 MODE32 MODE64
sgdt PTR test					; MODE16 MODE32 MODE64
; TODO: SHA1RNDS4
sha1nexte xmm0, XMMWORD PTR test		; MODE32 MODE64
sha1msg1 xmm0, XMMWORD PTR test			; MODE32 MODE64
sha1msg2 xmm0, XMMWORD PTR test			; MODE32 MODE64
sha256rnds2 xmm0, XMMWORD PTR test		; MODE32 MODE64
sha256msg1 xmm0, XMMWORD PTR test		; MODE32 MODE64
sha256msg2 xmm0, XMMWORD PTR test		; MODE32 MODE64
; TODO: SHLD
; TODO: SHRD
; TODO: SHUFPD
; TODO: SHUFPS
sidt PTR test					; MODE16 MODE32 MODE64
sldt PTR test					; MODE16 MODE32 MODE64
smsw PTR test					; MODE16 MODE32 MODE64
sqrtpd xmm0, XMMWORD PTR test			; MODE32 MODE64
sqrtps xmm0, XMMWORD PTR test			; MODE32 MODE64
sqrtsd xmm0, XMMWORD PTR test			; MODE32 MODE64
sqrtss xmm0, XMMWORD PTR test			; MODE32 MODE64
stac						; MODE32 MODE64
stc						; MODE16 MODE32 MODE64
std						; MODE16 MODE32 MODE64
std						; MODE16 MODE32 MODE64
stmxcsr PTR test				; MODE32 MODE64
stosb						; MODE16 MODE32 MODE64
stosw						; MODE16 MODE32 MODE64
stosd						; MODE16 MODE32 MODE64
stosq						; MODE64
str PTR test					; MODE16 MODE32 MODE64
sub al, 0x56					; MODE16 MODE32 MODE64
sub ax, 0x567					; MODE16 MODE32 MODE64
sub rax, 0x567					; MODE16 MODE32 MODE64
sub eax, 0x567					; MODE64
sub BYTE PTR test, 0x56				; MODE16 MODE32 MODE64
sub WORD PTR test, 0x567			; MODE16 MODE32 MODE64
sub DWORD PTR test, 0x567			; MODE16 MODE32 MODE64
sub QWORD PTR test, 0x567			; MODE64
sub WORD PTR test, 0x56				; MODE16 MODE32 MODE64
sub DWORD PTR test, 0x56			; MODE16 MODE32 MODE64
sub QWORD PTR test, 0x56			; MODE64
sub BYTE PTR test, al				; MODE16 MODE32 MODE64
sub WORD PTR test, ax				; MODE16 MODE32 MODE64
sub DWORD PTR test, eax				; MODE16 MODE32 MODE64
sub QWORD PTR test, rax				; MODE64
sub al, BYTE PTR test				; MODE16 MODE32 MODE64
sub ax, WORD PTR test				; MODE16 MODE32 MODE64
sub eax, DWORD PTR test				; MODE16 MODE32 MODE64
sub rax, QWORD PTR test				; MODE64
subpd xmm0, XMMWORD PTR test			; MODE32 MODE64
subps xmm0, XMMWORD PTR test			; MODE32 MODE64
subsd xmm0, XMMWORD PTR test			; MODE32 MODE64
subss xmm0, XMMWORD PTR test			; MODE32 MODE64
swapgs						; MODE64
syscall						; MODE64
sysenter					; MODE16 MODE32 MODE64
sysexit						; MODE16 MODE32 MODE64
sysexit32					; MODE16 MODE32 MODE64
sysexit64					; MODE16 MODE32 MODE64
sysret						; MODE16 MODE32 MODE64
sysret32					; MODE16 MODE32 MODE64
sysret64					; MODE16 MODE32 MODE64