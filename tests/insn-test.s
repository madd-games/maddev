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
call BYTE PTR test				; MODE16 MODE32 MODE64
call WORD PTR test				; MODE16 MODE32 MODE64
call DWORD PTR test				; MODE16 MODE32 MODE64
call QWORD PTR test				; MODE64
callf DWORD PTR test				; MODE16 MODE32
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
cmova al BYTE PTR test				; MODE16 MODE32 MODE64
cmova ax WORD PTR test				; MODE16 MODE32 MODE64
cmova eax DWORD PTR test			; MODE16 MODE32 MODE64
cmova rax QWORD PTR test			; MODE64
cmovae al BYTE PTR test				; MODE16 MODE32 MODE64
cmovae ax WORD PTR test				; MODE16 MODE32 MODE64
cmovae eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovae rax QWORD PTR test			; MODE64
cmovb al BYTE PTR test				; MODE16 MODE32 MODE64
cmovb ax WORD PTR test				; MODE16 MODE32 MODE64
cmovb eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovb rax QWORD PTR test			; MODE64
cmovbe al BYTE PTR test				; MODE16 MODE32 MODE64
cmovbe ax WORD PTR test				; MODE16 MODE32 MODE64
cmovbe eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovbe rax QWORD PTR test			; MODE64
cmovc al BYTE PTR test				; MODE16 MODE32 MODE64
cmovc ax WORD PTR test				; MODE16 MODE32 MODE64
cmovc eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovc rax QWORD PTR test			; MODE64
cmove al BYTE PTR test				; MODE16 MODE32 MODE64
cmove ax WORD PTR test				; MODE16 MODE32 MODE64
cmove eax DWORD PTR test			; MODE16 MODE32 MODE64
cmove rax QWORD PTR test			; MODE64
cmovg al BYTE PTR test				; MODE16 MODE32 MODE64
cmovg ax WORD PTR test				; MODE16 MODE32 MODE64
cmovg eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovg rax QWORD PTR test			; MODE64
cmovge al BYTE PTR test				; MODE16 MODE32 MODE64
cmovge ax WORD PTR test				; MODE16 MODE32 MODE64
cmovge eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovge rax QWORD PTR test			; MODE64
cmovl al BYTE PTR test				; MODE16 MODE32 MODE64
cmovl ax WORD PTR test				; MODE16 MODE32 MODE64
cmovl eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovl rax QWORD PTR test			; MODE64
cmovle al BYTE PTR test				; MODE16 MODE32 MODE64
cmovle ax WORD PTR test				; MODE16 MODE32 MODE64
cmovle eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovle rax QWORD PTR test			; MODE64
cmovna al BYTE PTR test				; MODE16 MODE32 MODE64
cmovna ax WORD PTR test				; MODE16 MODE32 MODE64
cmovna eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovna rax QWORD PTR test			; MODE64
cmovnae al BYTE PTR test			; MODE16 MODE32 MODE64
cmovnae ax WORD PTR test			; MODE16 MODE32 MODE64
cmovnae eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnae rax QWORD PTR test			; MODE64
cmovnb al BYTE PTR test				; MODE16 MODE32 MODE64
cmovnb ax WORD PTR test				; MODE16 MODE32 MODE64
cmovnb eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnb rax QWORD PTR test			; MODE64
cmovnbe al BYTE PTR test			; MODE16 MODE32 MODE64
cmovnbe ax WORD PTR test			; MODE16 MODE32 MODE64
cmovnbe eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnbe rax QWORD PTR test			; MODE64
cmovnc al BYTE PTR test				; MODE16 MODE32 MODE64
cmovnc ax WORD PTR test				; MODE16 MODE32 MODE64
cmovnc eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnc rax QWORD PTR test			; MODE64
cmovne al BYTE PTR test				; MODE16 MODE32 MODE64
cmovne ax WORD PTR test				; MODE16 MODE32 MODE64
cmovne eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovne rax QWORD PTR test			; MODE64
cmovng al BYTE PTR test				; MODE16 MODE32 MODE64
cmovng ax WORD PTR test				; MODE16 MODE32 MODE64
cmovng eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovng rax QWORD PTR test			; MODE64
cmovnge al BYTE PTR test			; MODE16 MODE32 MODE64
cmovnge ax WORD PTR test			; MODE16 MODE32 MODE64
cmovnge eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnge rax QWORD PTR test			; MODE64
cmovnl al BYTE PTR test				; MODE16 MODE32 MODE64
cmovnl ax WORD PTR test				; MODE16 MODE32 MODE64
cmovnl eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnl rax QWORD PTR test			; MODE64
cmovnle al BYTE PTR test			; MODE16 MODE32 MODE64
cmovnle ax WORD PTR test			; MODE16 MODE32 MODE64
cmovnle eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnle rax QWORD PTR test			; MODE64
cmovno al BYTE PTR test				; MODE16 MODE32 MODE64
cmovno ax WORD PTR test				; MODE16 MODE32 MODE64
cmovno eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovno rax QWORD PTR test			; MODE64
cmovnp al BYTE PTR test				; MODE16 MODE32 MODE64
cmovnp ax WORD PTR test				; MODE16 MODE32 MODE64
cmovnp eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnp rax QWORD PTR test			; MODE64
cmovns al BYTE PTR test				; MODE16 MODE32 MODE64
cmovns ax WORD PTR test				; MODE16 MODE32 MODE64
cmovns eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovns rax QWORD PTR test			; MODE64
cmovnz al BYTE PTR test				; MODE16 MODE32 MODE64
cmovnz ax WORD PTR test				; MODE16 MODE32 MODE64
cmovnz eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovnz rax QWORD PTR test			; MODE64
cmovo al BYTE PTR test				; MODE16 MODE32 MODE64
cmovo ax WORD PTR test				; MODE16 MODE32 MODE64
cmovo eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovo rax QWORD PTR test			; MODE64
cmovp al BYTE PTR test				; MODE16 MODE32 MODE64
cmovp ax WORD PTR test				; MODE16 MODE32 MODE64
cmovp eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovp rax QWORD PTR test			; MODE64
cmovpe al BYTE PTR test				; MODE16 MODE32 MODE64
cmovpe ax WORD PTR test				; MODE16 MODE32 MODE64
cmovpe eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovpe rax QWORD PTR test			; MODE64
cmovpo al BYTE PTR test				; MODE16 MODE32 MODE64
cmovpo ax WORD PTR test				; MODE16 MODE32 MODE64
cmovpo eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovpo rax QWORD PTR test			; MODE64
cmovs al BYTE PTR test				; MODE16 MODE32 MODE64
cmovs ax WORD PTR test				; MODE16 MODE32 MODE64
cmovs eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovs rax QWORD PTR test			; MODE64
cmovz al BYTE PTR test				; MODE16 MODE32 MODE64
cmovz ax WORD PTR test				; MODE16 MODE32 MODE64
cmovz eax DWORD PTR test			; MODE16 MODE32 MODE64
cmovz rax QWORD PTR test			; MODE64
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
; TODO: cmpss
cmpxchg BYTE PTR test, al			; MODE16 MODE32 MODE64
cmpxchg WORD PTR test, ax			; MODE16 MODE32 MODE64
cmpxchg DWORD PTR test, eax			; MODE16 MODE32 MODE64
cmpxchg QWORD PTR test, rax			; MODE64
cmpcchg8b BYTE PTR test				; MODE16 MODE32 MODE64
cmpcchg8b WORD PTR test				; MODE16 MODE32 MODE64
cmpcchg8b DWORD PTR test			; MODE16 MODE32 MODE64
cmpcchg8b QWORD PTR test			; MODE16 MODE32 MODE64
cmpcchg16b QWORD PTR test			; MODE64
comisd xmm1, XMMWORD PTR test			; MODE32 MODE64
cpuid						; MODE16 MODE32 MODE64
cvtdq2pd xmm2, XMMWORD PTR test			; MODE32 MODE64
cvtdq2ps xmm3, XMMWORD PTR test			; MODE32 MODE64
cvtpd2dq xmm4, XMMWORD PTR test			; MODE32 MODE64
cvtpd2pi XMMWORD PTR test, XMMWORD PTR test	; MODE16 MODE32 MODE64
cvtpd2ps xmm5, XMMWORD PTR test			; MODE32 MODE64
cvtpi2pd xmm6, XMMWORD PTR test			; MODE32 MODE64
cvtpi2ps xmm7, XMMWORD PTR test			; MODE32 MODE64
cvtpi2ps xmm0, XMMWORD PTR test			; MODE32 MODE64
cvtps2dq xmm1, XMMWORD PTR test			; MODE32 MODE64
cvtps2pd xmm2, XMMWORD PTR test			; MODE32 MODE64
cvtps2pi XMMWORD PTR test, XMMWORD PTR test	; MODE16 MODE32 MODE64
cvtsd2si xmm3, XMMWORD PTR test			; MODE32 MODE64
cvtsd2ss xmm4, XMMWORD PTR test			; MODE32 MODE64
cvtsi2sd xmm5, XMMWORD PTR test			; MODE32 MODE64
cvtsi2ss xmm6, XMMWORD PTR test			; MODE32 MODE64
cvtss2sd xmm7, XMMWORD PTR test			; MODE32 MODE64
cvtss2si xmm0, XMMWORD PTR test			; MODE32 MODE64
cvttpd2dq xmm1, XMMWORD PTR test		; MODE32 MODE64
cvttpd2pi XMMWORD PTR test, XMMWORD PTR test	; MODE16 MODE32 MODE64
cvttps2dq xmm2, XMMWORD PTR test		; MODE32 MODE64
cvttps2pi XMMWORD PTR test, XMMWORD PTR test	; MODE16 MODE32 MODE64
cvttsd2si xmm3, XMMWORD PTR test		; MODE32 MODE64
cvttss2si xmm4, XMMWORD PTR test		; MODE32 MODE64
cwd						; MODE16 MODE32 MODE64
cdq						; MODE16 MODE32 MODE64
cqo						; MODE64