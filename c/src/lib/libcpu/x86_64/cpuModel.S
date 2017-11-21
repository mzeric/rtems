/*  cpuModel.S
 *
 *  This file contains all assembly code for the Intel Cpu identification.
 *  It is based on linux cpu detection code.
 *
 *  Intel also provides public similar code in the book
 *  called :
 *
 *	Pentium Processor Family
 *		Developer Family
 *	Volume  3 :	Architecture and Programming Manual
 *
 * At the following place :
 *
 *	Chapter 5 :	Feature determination
 *	Chapter 25:	CPUID instruction
 *
 *  COPYRIGHT (c) 1998 valette@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/asm.h>
