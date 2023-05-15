	.text
	.file	"inside.c"
	.globl	checkPixel                      ; -- Begin function checkPixel
	.type	checkPixel,@function
checkPixel:                             ; @checkPixel
; %bb.0:                                ; %entry
	addi	sp, sp, -32
	sw	ra, 28(sp)                      ; 4-byte Folded Spill
	sw	s0, 24(sp)                      ; 4-byte Folded Spill
	addi	s0, sp, 32
	sw	a0, -12(s0)
	sw	a1, -16(s0)
	sw	a2, -20(s0)
	sw	a3, -24(s0)
	sw	a4, -28(s0)
	sw	a5, -32(s0)
	lw	s0, -12(s0)
	lw	a0, -16(s0)
	lw	a1, -20(s0)
	add	a0, a0, a1
	lw	a1, -24(s0)
	add	a0, a0, a1
	lw	a1, -28(s0)
	add	a0, a0, a1
	slli	a1, s0, 2
	sub	a0, a0, a1
	lw	a1, -32(s0)
	call	__divsi3@plt
	add	a0, s0, a0
	lw	s0, 24(sp)                      ; 4-byte Folded Reload
	lw	ra, 28(sp)                      ; 4-byte Folded Reload
	addi	sp, sp, 32
	jalr	zero, 0(ra)
.Lfunc_end0:
	.size	checkPixel, .Lfunc_end0-checkPixel
                                        ; -- End function
	.globl	main                            ; -- Begin function main
	.type	main,@function
main:                                   ; @main
; %bb.0:                                ; %entry
	addi	sp, sp, -12
	sw	ra, 8(sp)                       ; 4-byte Folded Spill
	sw	s0, 4(sp)                       ; 4-byte Folded Spill
	addi	s0, sp, 12
	sw	zero, -12(s0)
	addi	a1, zero, 1
	addi	a2, zero, 4
	addi	a3, zero, 3
	addi	a4, zero, 2
	addi	a5, zero, 6
	addi	a0, zero, 0
	call	checkPixel
	addi	a0, zero, 0
	lw	s0, 4(sp)                       ; 4-byte Folded Reload
	lw	ra, 8(sp)                       ; 4-byte Folded Reload
	addi	sp, sp, 12
	jalr	zero, 0(ra)
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        ; -- End function
	.ident	"clang version 15.0.0 (https://github.com/MarkGoncharovAl/llvm-project d481c6a639cc05184087c6d370da0d4687839cd8)"
	.section	".note.GNU-stack","",@progbits
