/**
 *  @file
 *
 *  @brief No CPU Dependent Source
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/types.h>
#include <inttypes.h>

void _CPU_Initialize(void)
{
  /*
   *  If there is not an easy way to initialize the FP context
   *  during Context_Initialize, then it is usually easier to
   *  save an "uninitialized" FP context here and copy it to
   *  the task's during Context_Initialize.
   */

  /* FP context initialization support goes here */
#ifdef __SSE__
  {
  uint64_t cr4;
    __asm__ __volatile__("mov %%cr4, %0":"=r"(cr4));
    if ( 0x600 != (cr4 & 0x600) ) {
      printk("PANIC: RTEMS was compiled for SSE but BSP did not enable it"
             "(CR4: 0%" PRIu32 ")\n", cr4);
      while ( 1 ) {
        __asm__ __volatile__("hlt");
	  }
	}
  }
#endif
}

uint32_t   _CPU_ISR_Get_level( void )
{
  /*
   *  This routine returns the current interrupt level.
   */

  uint32_t   level;

  do {
    register uint64_t   _eflags;
  
    __asm__ volatile ( "pushf\n"
                    "popq %0"
                    : "=rm" ((_eflags))
    );
    level = (_eflags & 0x200) ? 0 : 1;
  } while (0);

  return level;
}

Context_Control_fp _CPU_Null_fp_context;
/*
 *  _CPU_ISR_install_raw_handler
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */


/*
 *  _CPU_Install_interrupt_stack
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */

void _CPU_Install_interrupt_stack( void )
{
}

/*
 *  _CPU_Thread_Idle_body
 *
 *  NOTES:
 *
 *  1. This is the same as the regular CPU independent algorithm.
 *
 *  2. If you implement this using a "halt", "idle", or "shutdown"
 *     instruction, then don't forget to put it in an infinite loop.
 *
 *  3. Be warned. Some processors with onboard DMA have been known
 *     to stop the DMA if the CPU were put in IDLE mode.  This might
 *     also be a problem with other on-chip peripherals.  So use this
 *     hook with caution.
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */

void *_CPU_Thread_Idle_body( uintptr_t ignored )
{

  for( ; ; )
    /* insert your "halt" instruction here */ ;
}
void _CPU_Context_Initialize_fp(Context_Control_fp **_dest ){

    *(*_dest) = _CPU_Null_fp_context;
}
/*
 * Hence we must initialize the stack as follows
 *
 *         [arg1          ]:  n/a
 *         [arg0 (aligned)]:  n/a
 *         [ret. addr     ]:  NULL
 * SP->    [jump-target   ]:  _entry_point
 *
 * When Context_switch returns it pops the _entry_point from
 * the stack which then finds a standard layout.
 */

void _CPU_Context_Initialize(
  Context_Control *the_context,
  void *_stack_base,
  size_t _size,
  uint32_t _isr,
  void (*_entry_point)( void ),
  bool is_fp,
  void *tls_area
)
{
  uint64_t _stack;
  uint64_t tcb;

  (void) is_fp; /* avoid warning for being unused */

  if ( _isr ) {
    the_context->rflags = 0x46;//CPU_EFLAGS_INTERRUPTS_OFF;
  } else {
    the_context->rflags = 0x86;//CPU_EFLAGS_INTERRUPTS_ON;
  }

  _stack  = ((uint32_t)(_stack_base)) + (_size);
  _stack &= ~ (CPU_STACK_ALIGNMENT - 1);
  _stack -= 2*sizeof(proc_ptr*); /* see above for why we need to do this */
  *((proc_ptr *)(_stack)) = (_entry_point);
  the_context->rbp     = (void *) 0;
  the_context->rsp     = (void *) _stack;

  if ( tls_area != NULL ) {
    //tcb = (uint32_t) _TLS_TCB_after_TLS_block_initialize( tls_area );
    tcb = tls_area; // FIXME
  } else {
    tcb = 0;
  }

  the_context->gs.raw = DATA32_DESCRIPTOR;
}
static void _defaultExcHandler (CPU_Exception_frame *ctx)
{
  rtems_fatal(
    RTEMS_FATAL_SOURCE_EXCEPTION,
    (rtems_fatal_code) ctx
  );
}
typedef void (*rtems_raw_irq_hdl) (void);
void create_interrupt_gate_descriptor(interrupt_descriptor_t *desc, rtems_raw_irq_hdl *isr){
	X64_INT_DESC(desc, (uint64_t)isr);
}

