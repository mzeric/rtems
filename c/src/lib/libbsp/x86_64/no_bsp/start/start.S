#include <rtems/asm.h>

.set STACK_SIZE, 0x1000

BEGIN_CODE

	PUBLIC (start)
	EXTERN (start64)
SYM (start):
	cli
	movq $_end, %rax
	addq $0x4000, %rax
	movq %rax, %rsp
	movq %rax, %rbp
	pushq $0
	popf
#ifdef __SSE__
	call SYM(enable_sse)
#endif
	call start64

PUBLIC(bsp_idle_thread)
SYM(bsp_idle_thread):
hltloop: hlt
	jmp hltloop

PUBLIC(debug_hold)
SYM(debug_hold):
	pushq %rax
	movq $0, %rax
0:
	test %rax, %rax
	jz 0b
	popq %rax
	ret

PUBLIC(_stack_size)
SYM(_stack_size):
	.long STACK_SIZE

PUBLIC (enable_sse)
SYM(enable_sse):
	movq     %cr4, %rax		# OK to enable now
	or      $0x600, %rax
	movq     %rax, %cr4
	ret
