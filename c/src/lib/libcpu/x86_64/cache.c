/*
 *  Cache Management Support Routines for the i386
 */

#include <rtems.h>
#include "cache_.h"
#include <rtems/score/cpu.h>
#include <libcpu/page.h>

void _CPU_disable_cache(void)
{
  rtems_cache_flush_entire_data();
}

/*
 * Enable the entire cache
 */

void _CPU_enable_cache(void)
{
  /*rtems_cache_flush_entire_data();*/
}

/*
 * CACHE MANAGER: The following functions are CPU-specific.
 * They provide the basic implementation for the rtems_* cache
 * management routines. If a given function has no meaning for the CPU,
 * it does nothing by default.
 *
 * FIXME: The routines below should be implemented per CPU,
 *        to accomodate the capabilities of each.
 */

#if defined(X86_64_CACHE_ALIGNMENT)
void _CPU_cache_flush_1_data_line(const void *d_addr) {}
void _CPU_cache_invalidate_1_data_line(const void *d_addr) {}
void _CPU_cache_freeze_data(void) {}
void _CPU_cache_unfreeze_data(void) {}
void _CPU_cache_invalidate_1_instruction_line ( const void *d_addr ) {}
void _CPU_cache_freeze_instruction(void) {}
void _CPU_cache_unfreeze_instruction(void) {}

void _CPU_cache_flush_entire_data(void)
{
  __asm__ volatile ("wbinvd");
}
void _CPU_cache_invalidate_entire_data(void)
{
  __asm__ volatile ("invd");
}

void _CPU_cache_enable_data(void)
{
        _CPU_enable_cache();
}

void _CPU_cache_disable_data(void)
{
        _CPU_disable_cache();
}

void _CPU_cache_invalidate_entire_instruction(void)
{
  __asm__ volatile ("invd");
}

void _CPU_cache_enable_instruction(void)
{
  _CPU_enable_cache();
}

void _CPU_cache_disable_instruction( void )
{
  _CPU_disable_cache();
}
#endif
