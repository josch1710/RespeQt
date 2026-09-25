;  printf.asm - Printf utility routines for Atari SpartaDOS
;
;  Origin:
;    Based on the RespeQt Printf library (`atari/rcl_SpartaDos/printf.asm`)
;    Copyright (c) 2016 by Jonathan Halliday <fjc@atari8.co.uk>
;
;  Modifications for PCLink Suite:
;    - Streamlined format parser: Removed unused multi-byte/long format specifiers
;      (%l, %u, 16-bit/32-bit formatting routines) to optimize code size and reduce
;      memory footprint for `pclink.com` and `pcltest.com`.
;    - Retained core format specifiers: %s (string), %c (char), %d (8-bit dec),
;      %x (8-bit hex), %% (escaped percent).
;    - Preserved in-line argument passing and direct Atari CIOV (channel 0) output.
;
;  License:
;    This program is free software; you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation; either version 2 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;

	.proc GetNextArg
	ldy ArgIndex
	lda (Temp1),y
	sta Temp3
	iny
	lda (Temp1),y
	sta Temp3+1
	iny
	sty ArgIndex
	ldy #0
	rts
	.endp

	.proc GetArgPointer
	jsr StrLen
	iny
	sty ArgIndex
	rts
	.endp

	.proc Printf
	pla
	clc
	adc #1
	tay
	pla
	adc #0
	tax
	tya
	jsr PrintfAX
Return
	lda Temp1
	clc
	adc ArgIndex
	sta Temp1
	bcc @+
	inc Temp1+1
@
	jmp (Temp1)
	.endp

	.proc StrLen
	ldy #$FF
@
	iny
	lda (Temp1),y
	bne @-
	rts
	.endp

	.proc PrintfAX
	stax Temp1
NoAX
	lda #0
	sta ArgIndex
	jsr GetArgPointer
	ldy #0
MainLoop
	lda (Temp1),y
	beq Exit
	cmp #'%'
	beq ParseFormat
	jsr PutChar
	iny
	bne MainLoop
Exit
	rts

ParseFormat
	iny
	lda (Temp1),y
	cmp #'s'
	beq PrintString
	cmp #'c'
	beq PrintChar
	cmp #'d'
	beq PrintDec
	cmp #'x'
	beq PrintHex
	cmp #'%'
	beq PrintPercent
	jmp MainLoop

PrintPercent
	lda #'%'
	jsr PutChar
	iny
	jmp MainLoop

PrintChar
	tya
	pha
	jsr GetNextArg
	lda (Temp3),y
	jsr PutChar
	pla
	tay
	iny
	jmp MainLoop

PrintString
	tya
	pha
	jsr GetNextArg
	ldy #0
@
	lda (Temp3),y
	beq DoneString
	jsr PutChar
	iny
	bne @-
DoneString
	pla
	tay
	iny
	jmp MainLoop

PrintHex
	tya
	pha
	jsr GetNextArg
	lda (Temp3),y
	jsr PutHexByte
	pla
	tay
	iny
	jmp MainLoop

PrintDec
	tya
	pha
	jsr GetNextArg
	lda (Temp3),y
	jsr PutDecByte
	pla
	tay
	iny
	jmp MainLoop
	.endp

	.proc PutHexByte
	pha
	lsr
	lsr
	lsr
	lsr
	jsr PutHexNibble
	pla
	and #$0F
	jsr PutHexNibble
	rts
	.endp

	.proc PutHexNibble
	cmp #10
	bcc @+
	adc #6
@
	adc #'0'
	jsr PutChar
	rts
	.endp

	.proc PutDecByte
	ldx #0
@
	cmp #100
	bcc @+
	sbc #100
	inx
	bne @-
@
	pha
	cpx #0
	beq @+
	txa
	ora #'0'
	jsr PutChar
@
	pla
	ldx #0
@
	cmp #10
	bcc @+
	sbc #10
	inx
	bne @-
@
	pha
	txa
	ora #'0'
	jsr PutChar
	pla
	ora #'0'
	jsr PutChar
	rts
	.endp

	.proc PutChar
	pha
	tya
	pha
	txa
	pha
	ldx #0
	lda #11
	sta iccom,x
	pla
	tay
	pla
	tax
	pla
	pha
	sta $034D
	lda #0
	sta icblen,x
	sta icblen+1,x
	jsr ciov
	pla
	rts
	.endp
