/*  x86_64.h
 *
 *  This file sets up basic CPU dependency settings based on
 *  compiler settings.  For example, it can determine if
 *  floating point is available.  This particular implementation
 *  is specified to the NO CPU port.
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 */

#ifndef _RTEMS_SCORE_X86_64_H
#define _RTEMS_SCORE_X86_64_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This file contains the information required to build
 *  RTEMS for a particular member of the NO CPU family.
 *  It does this by setting variables to indicate which
 *  implementation dependent features are present in a particular
 *  member of the family.
 *
 *  This is a good place to list all the known CPU models
 *  that this port supports and which RTEMS CPU model they correspond
 *  to.
 */


#if defined(__x86_64__)

#define CPU_MODEL_NAME  "x86_64"
#define NOCPU_HAS_FPU     1

#else

#error "Unsupported CPU Model"

#endif

/*
 *  Define the name of the CPU family.
 */

#define CPU_NAME "Intel x86_64"

#ifndef ASM


typedef union{
	struct {
		uint16_t	privilege:2;
		uint16_t 	table:1; /* gdt(0) or ldt(1) */
		uint16_t 	index:13; /* bits 3 - 15 index in descriptor table */
	} RTEMS_PACKED;
	uint16_t raw;
} RTEMS_PACKED segment_selector;

typedef union {
   uint64_t raw;

   struct
   {
      uint64_t    limit_15_0:16;          /* bits 00-15 of the segment limit */
      uint64_t    base_address_15_0:16;           /* bits 00-15 of the base address */
      uint64_t    base_address_23_16:8;            /* bits 16-23 of the base address */
      uint64_t    type:4;              /* segment type */
      uint64_t    descriptor_type:1;                 /* descriptor type */
      uint64_t    privilege:2;               /* descriptor privilege level */
      uint64_t    present:1;                 /* segment present flag */
      uint64_t    limit_19_16:4;           /* bits 16-19 of the segment limit */
      uint64_t    available:1;               /* available for fun and profit */
      uint64_t    l:1;                 /* longmode */
      uint64_t    sz:1;                 /* default length, depend on seg type */
      uint64_t    gr:1;                 /* granularity */
      uint64_t    base_address_31_24:8;            /* bits 24-31 of the base address */

   } RTEMS_PACKED;

}  RTEMS_PACKED segment_descriptor;

typedef struct {
	segment_descriptor legacy;
	uint32_t 	base_address_63_32;
	uint64_t 	zero;
} 	RTEMS_PACKED segment_descriptor_64; /* tss ldt descriptor */
#define NULL_DESCRIPTOR 	(0x0000000000000000ULL)
#define CODE32_DESCRIPTOR 	(0x00cf9b000000ffffULL)
#define CODE64_DESCRIPTOR	(0x00af9b000000ffffULL)
#define DATA32_DESCRIPTOR	(0x00cf93000000ffffULL)

typedef struct{
	uint16_t limit;
	union{
		uint64_t addr;
		segment_descriptor *desc_32;
		segment_descriptor_64 *desc;
	};
}	RTEMS_PACKED gdt_reg_t;

typedef union{
	uint64_t raw;
	struct {
		uint64_t 	offset_15_0:16;
		uint64_t 	selector:16;
		uint64_t 	ist:3;
		uint64_t 	zero_1:5;
		uint64_t 	type:4;
		uint64_t 	zero_2:1;
		uint64_t 	privilege:2;
		uint64_t 	present:1;
		uint64_t 	offset_31_16:16;

	}	RTEMS_PACKED;
}	RTEMS_PACKED interrupt_descriptor_32_t;
typedef struct {
	interrupt_descriptor_32_t legacy;
	uint32_t 	offset_63_32;
	uint32_t 	zero;
} 	RTEMS_PACKED interrupt_descriptor_t;

typedef struct{
	uint16_t limit; /* limit = size - 1 */
	union{
		uint64_t addr;
		uint32_t saddr;
	};
} RTEMS_PACKED idt_reg_t;
/*
** Segment Selector
*/
#define SEG_SEL_GDT    0
#define SEG_SEL_LDT    1

#define SEG_SEL_KRN    0
#define SEG_SEL_USR    3

#define dt_seg_sel(idx,ti,rpl)      (((idx)<<3)|((ti)<<2)|(rpl))
#define gdt_seg_sel(idx,rpl)        dt_seg_sel(idx,SEG_SEL_GDT,rpl)
#define ldt_seg_sel(idx,rpl)        dt_seg_sel(idx,SEG_SEL_LDT,rpl)
#define gdt_krn_seg_sel(idx)        gdt_seg_sel(idx,SEG_SEL_KRN)
#define gdt_usr_seg_sel(idx)        gdt_seg_sel(idx,SEG_SEL_USR)

#define CODE_SEG_INDEX                 1
#define DATA_SEG_INDEX                 2
#define TSS_SEG_INDEX                  3

#define CODE_SELECTOR                gdt_krn_seg_sel(CODE_SEG_INDEX)
#define DATE_SELECTOR                dt_seg_sel(DATA_SEG_INDEX, SEG_SEL_GDT, SEG_SEL_KRN)
#define tss_sel                      gdt_krn_seg_sel(TSS_SEG_INDEX)

#define GDT_NR_DESC                  1+1+1+2*1 /* null:code:data:tss64 */

/*
** Segment Descriptor
*/
#define SEG_DESC_DATA_R             0x0
#define SEG_DESC_DATA_RA            0x1
#define SEG_DESC_DATA_RW            0x2
#define SEG_DESC_DATA_RWA           0x3
#define SEG_DESC_DATA_ER            0x4
#define SEG_DESC_DATA_ERA           0x5
#define SEG_DESC_DATA_ERW           0x6
#define SEG_DESC_DATA_ERWA          0x7

#define SEG_DESC_CODE_X             0x8
#define SEG_DESC_CODE_XA            0x9
#define SEG_DESC_CODE_XR            0xa
#define SEG_DESC_CODE_XRA           0xb
#define SEG_DESC_CODE_CX            0xc
#define SEG_DESC_CODE_CXA           0xd
#define SEG_DESC_CODE_CXR           0xe
#define SEG_DESC_CODE_CXRA          0xf


#define SEG_DESC_SYS_TSS_AVL_16     0x1
#define SEG_DESC_SYS_LDT            0x2
#define SEG_DESC_SYS_TSS_BUSY_16    0x3
#define SEG_DESC_SYS_CALL_GATE_16   0x4
#define SEG_DESC_SYS_TASK_GATE      0x5
#define SEG_DESC_SYS_INTR_GATE_16   0x6
#define SEG_DESC_SYS_TRAP_GATE_16   0x7
#define SEG_DESC_SYS_TSS_AVL_32     0x9
#define SEG_DESC_SYS_TSS_BUSY_32    0xb
#define SEG_DESC_SYS_CALL_GATE   0xc
#define SEG_DESC_SYS_INTR_GATE   0xe
#define SEG_DESC_SYS_TRAP_GATE   0xf

#define SEG_DESC_SYS_LDT_64         0x2
#define SEG_DESC_SYS_TSS_AVL_64     0x9
#define SEG_DESC_SYS_TSS_BUSY_64    0xb
#define SEG_DESC_SYS_CALL_GATE   0xc
#define SEG_DESC_SYS_INTR_GATE   0xe
#define SEG_DESC_SYS_TRAP_GATE   0xf

#define get_gdtr(aLocation)       asm volatile ("sgdt %0"::"m"(aLocation):"memory")
#define get_ldtr(aLocation)       asm volatile ("sldtl %0"::"m"(aLocation):"memory")
#define x64_get_idtr(aLocation)       asm volatile ("sidtl %0"::"m"(aLocation):"memory")
#define get_tr(aLocation)         asm volatile ("str   %0"::"m"(aLocation):"memory")

#define set_gdtr(val)             asm volatile ("lgdt  %0"::"m"(val):"memory")
#define set_ldtr(val)             asm volatile ("lldt  %0"::"m"(val):"memory")

#define set_tr(val)               asm volatile ("ltr   %%ax"::"a"(val))

#define farjump(_fptr)            asm volatile ("ljmp  *%0"::"m"(_fptr):"memory");
#define set_cs_eip(_cs,_eip)      asm volatile ("ljmp  %0, %1"::"i"(_cs), "i"(_eip))

#define x64_set_idtr(val)             asm volatile ("lidt  %0"::"m"(val):"memory")
#define LOW16(_x_) 		((uint16_t)((_x_)&0xffff))
#define LOW32(_x_)		((uint32_t)((_x_)&0xffffffff))
typedef struct far_ptr64
{
 uint64_t offset;
 uint16_t segment;

} __attribute__((packed)) fptr64_t;
#define set_cs(_cs)                                             \
   ({                                                           \
      fptr64_t  addr;                                            \
      addr.segment = _cs;                                       \
      asm volatile (                                            \
         "leaq  1f(%%rip), %%rax  \n"                           \
         "movq  %%rax, %0         \n"                           \
         "rex.w ljmp  *%0         \n"                           \
         "1:"                                                   \
         :"=m"(addr.offset)::"rax","memory");                   \
   })

#define segment_reload(_cs,_ds)                  \
   ({                                           \
    set_cs(_cs);				\
      asm volatile (                            \
         "movw   %%ax, %%ss  \n"                \
         "movw   %%ax, %%ds  \n"                \
         "movw   %%ax, %%es  \n"                \
         "movw   %%ax, %%fs  \n"                \
         "movw   %%ax, %%gs  \n"                \
         ::"a"(_ds));                           \
   })


#define int32_desc(_dsc_, _cs_, _isr_)                                  \
   ({                                                                   \
      (_dsc_)->offset_15_0		= LOW16(_isr_);                         \
      (_dsc_)->selector = _cs_;                                         \
      (_dsc_)->type     = SEG_DESC_SYS_INTR_GATE;	                    \
      (_dsc_)->offset_31_16 = ((_isr_)>>16);                            \
      (_dsc_)->present  = 1;                                            \
   })

#define int64_desc(_desc_, _cs_, _isr_)				\
	({												\
		(_desc_)->offset_63_32 = ((_isr_)>>32);		\
		interrupt_descriptor_32_t *legacy = &(_desc_)->legacy; \
		int32_desc(legacy, _cs_, LOW32(_isr_));		\
	})

#define X64_INT_DESC(desc, isr)		int64_desc(desc, CODE_SELECTOR, isr)
RTEMS_INLINE_ROUTINE void x64_set_idt_handler(interrupt_descriptor_t *idt, int index, uint64_t isr){
		X64_INT_DESC(&idt[index], isr);
}
RTEMS_INLINE_ROUTINE void intr(uint8_t num) {
  __asm__ volatile("int %0;" :: "i"(num));
}
RTEMS_INLINE_ROUTINE uint64_t x86_64_base_gdt_entry (segment_descriptor* gdt_entry)
{
    return (gdt_entry->base_address_15_0 |
            (gdt_entry->base_address_23_16<<16) |
            (gdt_entry->base_address_31_24<<24));	
}

#define __cpuid_1(idx,eax)                                              \
   asm volatile ("cpuid":"=a"(eax):"a"(idx):"ebx","ecx","edx")
/*
** CPUID Maximum supported extension
*/
#define CPUID_MAX_EXT                   0x80000000
#define cpuid_max_ext()                 \
   ({                                   \
      uint32_t eax;                     \
      __cpuid_1(CPUID_MAX_EXT,eax);     \
      eax;                              \
   })


#endif // ! ASM
#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_SCORE_X86_64_H */
