#include <rtems/asm.h>
#include <rtems/score/cpu.h>



.set REG_RFLAGS, X64_CONTEXT_CONTROL_RFLAGS_OFFSET
.set REG_RSP, X64_CONTEXT_CONTROL_RSP_OFFSET
.set REG_RBP, X64_CONTEXT_CONTROL_RBP_OFFSET
.set REG_RBX, X64_CONTEXT_CONTROL_RBX_OFFSET
.set REG_RSI, X64_CONTEXT_CONTROL_RSI_OFFSET
.set REG_RDI, X64_CONTEXT_CONTROL_RDI_OFFSET
.set REG_GS,  X64_CONTEXT_CONTROL_GS_OFFSET

        BEGIN_CODE


        .p2align  4

        PUBLIC (_CPU_Context_switch)

.set RUNCONTEXT_ARG,   8                   /* save context argument */
.set HEIRCONTEXT_ARG,  16                   /* restore context argument */

SYM (_CPU_Context_switch):
        movq      RUNCONTEXT_ARG(%rsp),%rax  /* eax = running threads context */
	movq	%rdi, %rax
        pushf                              /* push eflags */
        popq      REG_RFLAGS(%rax)          /* save eflags */
        movq      %rsp,REG_RSP(%rax)         /* save stack pointer */
        movq      %rbp,REG_RBP(%rax)         /* save base pointer */
        movq      %rbx,REG_RBX(%rax)         /* save ebx */
        movq      %rsi,REG_RSI(%rax)         /* save source register */
        movq      %rdi,REG_RDI(%rax)         /* save destination register */

#ifdef RTEMS_SMP
        /* The executing context no longer executes on this processor */
        movb      $0, I386_CONTEXT_CONTROL_IS_EXECUTING_OFFSET(eax)
#endif

        movq      HEIRCONTEXT_ARG(%rsp),%rax /* eax = heir threads context */
        movq      %rsi, %rax


restore:
        pushq     REG_RFLAGS(%rax)          /* push eflags */
        popf                               /* restore eflags */
        movq      REG_RSP(%rax),%rsp         /* restore stack pointer */
        movq      REG_RBP(%rax),%rbp         /* restore base pointer */
        movq      REG_RBX(%rax),%rbx         /* restore ebx */
        movq      REG_RSI(%rax),%rsi         /* restore source register */
        movq      REG_RDI(%rax),%rdi         /* restore destination register */
        movq      REG_GS(%rax), %rcx       /* restore gs segment */

        //movq      %rcx, __gdt__ + 24

        //movq      $24, %rcx
        //mov       rcx, gs
        retq
       
        PUBLIC (_CPU_Context_restore)

.set NEWCONTEXT_ARG,   8                   /* context to restore argument */

SYM (_CPU_Context_restore):
        movq      NEWCONTEXT_ARG(%rsp),%rax  /* eax = running threads context */
	movq	%rdi, %rax
        jmp       restore


#define DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY(_vector) \
        .p2align  16                        ; \
        PUBLIC (rtems_exception_prologue_ ## _vector ) ; \
SYM (rtems_exception_prologue_ ## _vector ):             \
	pushq	$ _vector	; \
        jmp   SYM (_Exception_Handler) ;

#define DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY(_vector) \
        .p2align  16                        ; \
        PUBLIC (rtems_exception_prologue_ ## _vector ) ; \
SYM (rtems_exception_prologue_ ## _vector ):             \
	pushq	$ 0		; \
	pushq	$ _vector	; \
        jmp   SYM (_Exception_Handler) ;

        PUBLIC (_Exception_Handler)
SYM (_Exception_Handler):
	//pushqa                  /* Push general purpose registers    */
	hlt
	add $16, %rsp
	iretq
PUBLIC(idt_exception)
SYM(idt_exception):
	cli
	push %rax
	push %rcx
	push %rbx
	push %rsp
	push %rbp
	push %rsi
	push %rdi
	mov  %rsp, %rdi
	call test_intr_79
	pop  %rdi
	pop  %rsi
	pop  %rbp
	pop  %rsp
	pop  %rbx
	pop  %rcx
	pop  %rax

	add $16, %rsp

	iretq

PUBLIC( rtems_exception_79)
SYM(rtems_exception_79):
.align 16
	pushq $-1
	pushq $79
	jmp idt_exception
/*
 * Divide Error
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (0)
/*
 * Debug Exception
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (1)
/*
 * NMI
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (2)
/*
 * Breakpoint
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (3)
/*
 * Overflow
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (4)
/*
 * Bound Range Exceeded
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (5)
/*
 * Invalid Opcode
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (6)
/*
 * No Math Coproc
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (7)
/*
 * Double Fault
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (8)
/*
 * Coprocessor segment overrun
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (9)
/*
 * Invalid TSS
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (10)
/*
 * Segment Not Present
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (11)
/*
 * Stack segment Fault
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (12)
/*
 * General Protection Fault
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (13)
/*
 * Page Fault
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (14)
/*
 * Floating point error (NB 15 is reserved it is therefor skipped)
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (16)
/*
 * Aligment Check
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (17)
/*
 * Machine Check
 */
DISTINCT_EXCEPTION_WITH_FAULTCODE_ENTRY (18)

#ifdef __SSE__
/*
 * SIMD FP Exception
 */
DISTINCT_EXCEPTION_WITHOUT_FAULTCODE_ENTRY (19)
#endif
