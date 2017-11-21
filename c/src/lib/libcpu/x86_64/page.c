/*
 * page.c :- This file contains implementation of C function to
 *           Instanciate paging. More detailled information
 *	     can be found on Intel site and more precisely in
 *           the following book :
 *
 *		Pentium Processor familly
 *		Developper's Manual
 *
 *		Volume 3 : Architecture and Programming Manual
 *
 * Copyright (C) 1999  Emmanuel Raguet (raguet@crf.canon.fr)
 *                     Canon Centre Recherche France.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtems.h>
#include <rtems/score/cpu.h>
#include <libcpu/page.h>

#define MEMORY_SIZE 0x4000000 		/* 64Mo */


extern uint32_t   bsp_mem_size;

/*************************************************************************/
/************** IT IS A ONE-TO-ONE TRANSLATION ***************************/
/*************************************************************************/


/*
 * Disable the paging
 */
void _CPU_disable_paging(void)
{
}

/*
 * Enable the paging
 */
void _CPU_enable_paging(void)
{
  rtems_cache_flush_entire_data();
}


/*
 * Initialize the paging with 1-to-1 mapping
 */

int init_paging(void)
{

  _CPU_enable_cache();
  _CPU_enable_paging();

  return 0;
}

/*
 * Is cache enable
 */
int  _CPU_is_cache_enabled(void)
{
	return 1;
}

/*
 * Is paging enable
 */
int  _CPU_is_paging_enabled(void)
{
	return 1;
}



/*
 * Translate the physical address in the virtual space and return
 * the translated address in mappedAddress
 */

int _CPU_map_phys_address(
  void **mappedAddress,
  void  *physAddress,
  int    size,
  int    flag
)
{
  return 0;
}

/*
 * "Compress" the Directory and Page tables to avoid
 * important loss of address range
 */


/*
 * Unmap the virtual address from the tables
 * (we do not deallocate the table already allocated)
 */

int _CPU_unmap_virt_address(
  void *mappedAddress,
  int   size
)
{

  return 0;
}

/*
 * Modify the flags PRESENT, WRITABLE, USER, WRITE_TROUGH, CACHE_DISABLE
 * of the page's descriptor.
 */

int _CPU_change_memory_mapping_attribute(
  void         **newAddress,
  void          *mappedAddress,
  unsigned int   size,
  unsigned int   flag
)
{

  return 0;
}

/*
 * Display the page descriptor flags
 * CACHE_DISABLE of the whole memory
 */

#include <rtems/bspIo.h>

int  _CPU_display_memory_attribute(void)
{

  return 0;
}
