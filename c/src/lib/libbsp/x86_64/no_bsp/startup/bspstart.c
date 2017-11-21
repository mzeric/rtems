/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <string.h>

#include <bsp.h>
#include <bsp/bootcard.h>

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
*/
#define __regparm__(_n_)          __attribute__ ((regparm((_n_))))
#define COM1_BASE 0x3F8
#define COM1_IRQ 4
#define COM2_IRQ 3
#define COM3_IRQ 4
#define COM4_IRQ 3

// and [here](http://www.lammertbies.nl/comm/info/serial-uart.html)
#define RX_FIFO 0
#define TX_FIFO 0

#define COM_TX_FIFO ((short int)(COM1_BASE + TX_FIFO))
static inline void outb(uint16_t port, uint8_t val) {
    __asm volatile ( "outb %0, %1" : : "a"(val), "d"(port) );
}

extern void debug_hold(void);/* in start.S, stop qemu, waiting for gdb remote connection */

static inline void serial_output_char(char ch) {
  outb(COM_TX_FIFO, ch);
}
static inline void serial_poll_char(void) {
    /* empty */
}
/* Global Descriptor Table Must be 8 byte aligned */
#define ALIGNED(_n_) __attribute__((__aligned__(_n_)))
__attribute__((__aligned__(8))) segment_descriptor __gdt__[GDT_NR_DESC];
__attribute__((__aligned__(8))) interrupt_descriptor_t __idt__[256];
extern void start(void);
__attribute__((section(".entry"), used)) void bin_entry(){
	__asm__ volatile ("jmp %0"::"m"(start));
}

static gdt_reg_t __gdt_reg_;

void interrupt_init();
void segment_init();
void page_init();

void __regparm__(1) start64(void){
	/*debug_hold(); */
	*(uint32_t*)0xb8000 = 0x2f4b2f4f;

	boot_card(NULL);
}

void bsp_start( void )
{

  serial_output_char('X');
  cpuid_max_ext();
  segment_init();
  page_init();
  interrupt_init();
  serial_output_char('I');
  serial_output_char('\n');
}
void _CPU_Fatal_halt(uint32_t source, uint32_t error)
{
  __asm__ volatile (
         "cli\n\t"
         "movl %0,%%eax\n\t"
         "hlt\n\t"
         : "=r" ((error))
         : "0" ((error))
  );
  #ifdef __GNUC__
    __builtin_unreachable();
  #endif
}
typedef struct {
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
}intr_frame_t;

#define INIT __attribute__((__interrupt__))

void test_intr_79(void ){
	//printf("test_init:%x\n", intr->ip);
    serial_output_char('I');
}
void test_init(){

    __asm__ volatile ("int $79");
    serial_output_char('R');
}
void x64_set_interrupt_descriptor(interrupt_descriptor_t *idt, uint64_t addr){

}
extern void rtems_exception_prologue_0(void);
extern void rtems_exception_prologue_1(void);
extern void rtems_exception_prologue_2(void);
extern void rtems_exception_prologue_3(void);
extern void rtems_exception_prologue_4(void);
extern void rtems_exception_prologue_5(void);
extern void rtems_exception_prologue_6(void);
extern void rtems_exception_prologue_7(void);
extern void rtems_exception_prologue_8(void);
extern void rtems_exception_prologue_9(void);
extern void rtems_exception_prologue_10(void);
extern void rtems_exception_prologue_11(void);
extern void rtems_exception_prologue_12(void);
extern void rtems_exception_prologue_13(void);
extern void rtems_exception_prologue_14(void);
extern void rtems_exception_prologue_16(void);
extern void rtems_exception_prologue_17(void);
extern void rtems_exception_prologue_18(void);
#ifdef __SSE__
extern void rtems_exception_prologue_19(void);
#endif
typedef void (*rtems_raw_irq_hdl) (void);
static rtems_raw_irq_hdl tbl[] = {
	 rtems_exception_prologue_0,
	 rtems_exception_prologue_1,
	 rtems_exception_prologue_2,
	 rtems_exception_prologue_3,
	 rtems_exception_prologue_4,
	 rtems_exception_prologue_5,
	 rtems_exception_prologue_6,
	 rtems_exception_prologue_7,
	 rtems_exception_prologue_8,
	 rtems_exception_prologue_9,
	 rtems_exception_prologue_10,
	 rtems_exception_prologue_11,
	 rtems_exception_prologue_12,
	 rtems_exception_prologue_13,
	 rtems_exception_prologue_14,
     0,
	 rtems_exception_prologue_16,
	 rtems_exception_prologue_17,
	 rtems_exception_prologue_18,
#ifdef __SSE__
	 rtems_exception_prologue_19,
#endif
};
extern void rtems_exception_79 (void);
#define ARRAY_SIZE(x) sizeof(x)/sizeof((x)[0])
void interrupt_init(){
    idt_reg_t idtr;
    uint64_t isr, level;

    for(int i = 0; i < ARRAY_SIZE(tbl); i++)
	create_interrupt_gate_descriptor(&__idt__[i], tbl[i]);

    create_interrupt_gate_descriptor(&__idt__[79], rtems_exception_79);
    _CPU_ISR_Disable(level);
    idtr.addr = __idt__;
    idtr.limit = sizeof(__idt__[0])*256 - 1;
    x64_set_idtr(idtr);
    _CPU_ISR_Enable(level);
    test_init();
    return;
}
void segment_init(){

    __gdt__[0].raw = NULL_DESCRIPTOR;
    __gdt__[CODE_SEG_INDEX].raw = CODE64_DESCRIPTOR;
    __gdt__[DATA_SEG_INDEX].raw = DATA32_DESCRIPTOR;

    __gdt_reg_.limit = sizeof(__gdt__) - 1;
    __gdt_reg_.addr = &__gdt__;

    set_gdtr(__gdt_reg_);
    //FIXME !! reset cs and ds
    segment_reload(CODE_SELECTOR, DATE_SELECTOR);
    gdt_reg_t tmp;
    get_gdtr(tmp);

    return;
}
/*
   ** Paging bits accessing
   */
#define PG_P_BIT        0
#define PG_RW_BIT       1
#define PG_LVL_BIT      2
#define PG_PWT_BIT      3
#define PG_PCD_BIT      4
#define PG_ACC_BIT      5
#define PG_DRT_BIT      6
#define PG_PAT_BIT      7
#define PG_GLB_BIT      8

#define PG_P            (1<<PG_P_BIT)
#define PG_RO           0
#define PG_RW           (1<<PG_RW_BIT)
#define PG_KRN          0
#define PG_USR          (1<<PG_LVL_BIT)
#define PG_PWT          (1<<PG_PWT_BIT)
#define PG_PCD          (1<<PG_PCD_BIT)
#define PG_ACC          (1<<PG_ACC_BIT)
#define PG_DRT          (1<<PG_DRT_BIT)
#define PG_PAT          (1<<PG_PAT_BIT)
#define PG_GLB          (1<<PG_GLB_BIT)

/*
   ** Large pages
   */
#define PG_PS_BIT       7
#define PG_LPAT_BIT     12

#define PG_PS           (1<<PG_PS_BIT)
#define PG_LPAT         (1<<PG_LPAT_BIT)

/*
   ** Long mode
   */
#define PG_NX_BIT       63
#define PG_NX           (1UL<<PG_NX_BIT)

#define valid_pae_pdpe(_p)                      \
       ({                                           \
       	      int x = 1;                                \
       	      if((_p)->pae.p)                           \
       	      {                                         \
       	         if((_p)->pae.r0 || (_p)->pae.r1)       \
       	            x = 0;                              \
       	         if((_p)->raw > info->vm.cpu.max_paddr) \
       	            x = 0;                              \
       	      }                                         \
       	      x;                                        \
       	   })

typedef union page_map_level_4_entry
{
    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  mbz:3;
	uint64_t  avl:3;
	uint64_t  addr:40; /* bit 12 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed));

    uint64_t raw;

} __attribute__((packed)) pml4e_t;

typedef union page_directory_pointer_entry
{
    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  mbz:3;
	uint64_t  avl:3;
	uint64_t  addr:40; /* bit 12 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed));

    struct
    {
	uint64_t  p:1;
	uint64_t  r0:2;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  r1:4;
	uint64_t  ign:3;
	uint64_t  addr:40; /* bit 12 */
	uint64_t  r2:12;

    } __attribute__((packed)) pae; /* specific pae pdpte register */
    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  d:1;
	uint64_t  ps:1;     /* bit 7 */
	uint64_t  g:1;
	uint64_t  avl:3;
	uint64_t  pat:1;
	uint64_t  mbz:17;
	uint64_t  addr:22;  /* bit 30 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed)) page;

      uint64_t raw;

} __attribute__((packed)) pdpe_t;

typedef union page_directory_entry_64
{
    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  mbz:3;
	uint64_t  avl:3;
	uint64_t  addr:40;   /* bit 12 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed));

    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  d:1;
	uint64_t  ps:1;      /* bit 7 */
	uint64_t  g:1;
	uint64_t  avl:3;
	uint64_t  pat:1;
	uint64_t  mbz:8;
	uint64_t  addr:31;   /* bit 21 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed)) page;
   
    uint64_t raw;
} __attribute__((packed)) pde64_t;
typedef union page_table_entry_64
{
    struct
    {
	uint64_t  p:1;
	uint64_t  rw:1;
	uint64_t  lvl:1;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  acc:1;
	uint64_t  d:1;
	uint64_t  pat:1;
	uint64_t  g:1;
	uint64_t  avl:3;
	uint64_t  addr:40;  /* bit 12 */
	uint64_t  avl2:11;
	uint64_t  nx:1;

    } __attribute__((packed));

    uint64_t raw;

} __attribute__((packed)) pte64_t;
/*
   ** 64 bits paging usefull macros
   */
#define PML4E_PER_PML4               512UL
#define PDPE_PER_PDP                 512UL
#define PDE64_PER_PD                 512UL
#define PTE64_PER_PT                 512UL

#define PG_2M_SHIFT                  21
#define PG_2M_SIZE                   (1UL<<PG_2M_SHIFT)
#define pg_2M_offset(addr)           ((addr)&(PG_2M_SIZE-1))
#define pg_2M_nr(addr)               ((addr)>>PG_2M_SHIFT)
#define pg_2M_addr(bits)             ((bits)<<PG_2M_SHIFT)
#define pg_2M_align(addr)            __align(addr,PG_2M_SIZE)
#define pg_2M_align_next(addr)       __align_next(addr,PG_2M_SIZE)
#define pg_2M_aligned(addr)          __aligned(addr,PG_2M_SIZE)

#define PG_1G_SHIFT                  30
#define PG_1G_SIZE                   (1UL<<PG_1G_SHIFT)
#define pg_1G_offset(addr)           ((addr)&(PG_1G_SIZE-1))
#define pg_1G_nr(addr)               ((addr)>>PG_1G_SHIFT)
#define pg_1G_addr(bits)             ((bits)<<PG_1G_SHIFT)
#define pg_1G_align(addr)            __align(addr,PG_1G_SIZE)
#define pg_1G_align_next(addr)       __align_next(addr,PG_1G_SIZE)
#define pg_1G_aligned(addr)          __aligned(addr,PG_1G_SIZE)

#define PG_512G_SHIFT                39
#define PG_512G_SIZE                 (1UL<<PG_512G_SHIFT)
#define pg_512G_offset(addr)         ((addr)&(PG_512G_SIZE-1))
#define pg_512G_nr(addr)             ((addr)>>PG_512G_SHIFT)
#define pg_512G_addr(bits)           ((bits)<<PG_512G_SHIFT)
#define pg_512G_align(addr)          __align(addr,PG_512G_SIZE)
#define pg_512G_align_next(addr)     __align_next(addr,PG_512G_SIZE)
#define pg_512G_aligned(addr)        __aligned(addr,PG_512G_SIZE)

/*
   ** Special pdp for pmode + pae
   */
#define PG_32B_SHIFT                 5
#define PG_32B_SIZE                  (1UL<<PG_32B_SHIFT)
#define pg_32B_addr(bits)            ((bits)<<PG_32B_SHIFT)

#define pdp_pae_idx(addr)            (((addr)>>PG_1G_SHIFT)&0x3)

/*
   ** relative index for paging walk through
   */
#define pml4_idx(addr)               (((addr)>>PG_512G_SHIFT)&0x1ff)
#define pdp_idx(addr)                (((addr)>>PG_1G_SHIFT)&0x1ff)
#define pd64_idx(addr)               (((addr)>>PG_2M_SHIFT)&0x1ff)
#define pt64_idx(addr)               (((addr)>>PG_4K_SHIFT)&0x1ff)

/*
   ** absolute index:
   **  - pml4_abs_idx == pdp table number
   **  - pdp_abs_idx  == pd  table number
   **  - pd_abs_idx   == pt  table number
   */
#define pg_abs_idx(addr,shift)       ((addr)>>(shift))
#define pdp_nr(addr)                 ((addr)>>PG_512G_SHIFT)
#define pd64_nr(addr)                ((addr)>>PG_1G_SHIFT)
#define pt64_nr(addr)                ((addr)>>PG_2M_SHIFT)

#define PML4_SIZE                      (PML4E_PER_PML4*sizeof(pml4e_t))
#define PDP_SIZE                       (PDPE_PER_PDP*sizeof(pdpe_t))
#define PD64_SIZE                      (PDE64_PER_PD*sizeof(pde64_t))
#define PT64_SIZE                      (PTE64_PER_PT*sizeof(pte64_t))

/*
   ** Universal macros
   */
#define pg_present(_e_)              ((_e_)->p)
#define pg_readable(_e_)             pg_present(_e_)
#define pg_writable(_e_)             (pg_present(_e_) && ((_e_)->rw))
#define pg_large(_e_)                ((_e_)->page.ps)
#define pg_zero(_e_)                 ((_e_)->raw = 0)

#define pg64_executable(_e_)         \
       ({ uint8_t x;                     \
       	      if(__efer.nxe)                 \
       	         x = !(_e_)->nx;             \
       	      else x = 1;                    \
       	      x;                             \
       	   })

#define pg_executable(_e_)                              \
       ({                                                   \
       	      uint8_t x;                                        \
       	      if(_xx_lmode() || __cr4.pae)                      \
       	         x = pg64_executable(_e_);                      \
       	      else                                              \
       	         x = pg_present(_e_);                           \
       	      x;                                                \
       	   })

/* XXX: what about NX bit ? */
#define pg_get_attr(_e_)             ((_e_)->raw & (PG_USR|PG_RW))
#define pg_set_pvl(_e_,_p_)          ({(_e_)->blow &= ~7;(_e_)->blow |= (_p_);})
#define pg_has_pvl_w(_e)             ((_e_)->blow & PG_RW)

#define pg_set_entry(_e_,_attr_,_pfn_)          \
       ({                                           \
       	      (_e_)->raw  = (_attr_)|PG_P;              \
       	      (_e_)->addr = _pfn_;                      \
       	   })

#define pg_set_large_entry(_e_,_attr_,_pfn_)    \
       ({                                           \
       	      (_e_)->raw       = (_attr_)|PG_PS|PG_P;   \
       	      (_e_)->page.addr = _pfn_;                 \
       	   })

/*
   ** CR0
   */
#define CR0_PE_BIT      0
#define CR0_MP_BIT      1
#define CR0_EM_BIT      2
#define CR0_TS_BIT      3
#define CR0_ET_BIT      4
#define CR0_NE_BIT      5
#define CR0_WP_BIT     16
#define CR0_AM_BIT     18
#define CR0_NW_BIT     29
#define CR0_CD_BIT     30
#define CR0_PG_BIT     31

#define CR0_PE         (1UL<<CR0_PE_BIT)
#define CR0_MP         (1UL<<CR0_MP_BIT)
#define CR0_EM         (1UL<<CR0_EM_BIT)
#define CR0_TS         (1UL<<CR0_TS_BIT)
#define CR0_ET         (1UL<<CR0_ET_BIT)
#define CR0_NE         (1UL<<CR0_NE_BIT)
#define CR0_WP         (1UL<<CR0_WP_BIT)
#define CR0_AM         (1UL<<CR0_AM_BIT)
#define CR0_NW         (1UL<<CR0_NW_BIT)
#define CR0_CD         (1UL<<CR0_CD_BIT)
#define CR0_PG         (1UL<<CR0_PG_BIT)
typedef union control_register_0
{
    struct
    {
	uint64_t    pe:1;   /* protected mode */
	uint64_t    mp:1;   /* monitor copro */
	uint64_t    em:1;   /* emulation */
	uint64_t    ts:1;   /* task switch */
	uint64_t    et:1;   /* ext type */
	uint64_t    ne:1;   /* num error */
	uint64_t    r1:10;  /* reserved */
	uint64_t    wp:1;   /* write protect */
	uint64_t    r2:1;   /* reserved */
	uint64_t    am:1;   /* align mask */
	uint64_t    r3:10;  /* reserved */
	uint64_t    nw:1;   /* not write through */
	uint64_t    cd:1;   /* cache disable */
	uint64_t    pg:1;   /* paging */

    } __attribute__((packed));

    uint64_t raw;

} __attribute__((packed)) cr0_reg_t;
/*
   ** XCR0
   */
typedef union extended_control_register_0
{
    struct
    {
	uint64_t    x87:1;   /* FPU/MMX state must be 1 */
	uint64_t    sse:1;   /* SSE state */
	uint64_t    avx:1;   /* AVX state */
	uint64_t    rsv:61;

    } __attribute__((packed));

    uint64_t raw;

} __attribute__((packed)) xcr0_reg_t;
/*
   ** CR3
 */
typedef union control_register_3
{
    struct
    {
	uint64_t  r1:3;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  r2:7;
	uint64_t  addr:20;

    } __attribute__((packed));

    struct
    {
	uint64_t  r1:3;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  addr:27;

    } __attribute__((packed)) pae;

    struct
    {
	uint64_t  r1:3;
	uint64_t  pwt:1;
	uint64_t  pcd:1;
	uint64_t  r2:7;
	uint64_t  addr:40; /* bit 12 */

    } __attribute__((packed)) pml4;

    uint64_t raw;

} __attribute__((packed)) cr3_reg_t;
typedef union control_register_4
{
    struct
    {
	uint64_t    vme:1;        /* virtual 8086 */
	uint64_t    pvi:1;        /* pmode virtual int */
	uint64_t    tsd:1;        /* time stamp disable */
	uint64_t    de:1;         /* debug ext */
	uint64_t    pse:1;        /* page sz ext */
	uint64_t    pae:1;        /* phys addr ext */
	uint64_t    mce:1;        /* machine check enable */
	uint64_t    pge:1;        /* page global enable */
	uint64_t    pce:1;        /* perf counter enable */
	uint64_t    osfxsr:1;     /* fxsave fxstore */
	uint64_t    osxmmexcpt:1; /* simd fpu excpt */
	uint64_t    r1:2;         /* reserved */
	uint64_t    vmxe:1;       /* vmx enable */
	uint64_t    smxe:1;       /* smx enable */
	uint64_t    r2:2;         /* reserved */
	uint64_t    pcide:1;      /* process context id */
	uint64_t    osxsave:1;    /* xsave enable */
	uint64_t    r3:1;         /* reserved */
	uint64_t    smep:1;       /* smep */
	uint64_t    r4:11;        /* smep reserved */

    } __attribute__((packed));

    uint64_t raw;

} __attribute__((packed)) cr4_reg_t;
#define get_cr(_n_)     ({ulong_t x;asm volatile("mov %%cr" #_n_ ", %0":"=r"(x));x;})
#define set_cr(_n_,_x_) asm volatile("mov %0, %%cr" #_n_ ::"r"(_x_))
#define enable_paging() ({ulong_t cr0 = get_cr(0); set_cr(0, cr0|CR0_PG);})

#define PG_4K_SHIFT                  12
#define PG_4K_SIZE                   (1UL<<PG_4K_SHIFT)
#define pg_4K_offset(addr)           ((addr)&(PG_4K_SIZE-1))
#define pg_4K_nr(addr)               ((addr)>>PG_4K_SHIFT)
#define page_nr(_x_) 			 pg_4K_nr((uint64_t)_x_)
#define __align(addr,val)      ((addr)&(~((val)-1)))
#define PAGE_SIZE                    PG_4K_SIZE
#define page_align(addr)             __align(addr,PAGE_SIZE)

/* init boot first pool */
typedef union pool_page_descriptor_entry
{
    uint16_t flags:12;
    size_t raw;

} __attribute__((packed)) pool_pg_desc_e_t;

typedef struct pool_page_descriptor
{
    union  pool_page_descriptor_entry page;
    struct pool_page_descriptor       *prev, *next;

} __attribute__((packed)) pool_pg_desc_t;

typedef struct pool_page_list
{
    pool_pg_desc_t *list;
    size_t         nr;

} __attribute__((packed)) pool_pg_list_t;

typedef struct pool_information
{
    size_t       addr;
    size_t         sz;
    pool_pg_desc_t *all, *used, *free;

} __attribute__((packed)) vmm_pool_t;
void     pool_init();

void     pool_push_page(size_t addr){

}
size_t pool_pop_page(){

}
/*
ALIGNED(4096) pml4e_t __pml4[PML4E_PER_PML4];
ALIGNED(4096) pdpe_t  __pdp[PDPE_PER_PDP];
ALIGNED(4096) pde64_t __pd[PDE64_PER_PD];
ALIGNED(4096) pte64_t __pt[PTE64_PER_PT];
*/
// not suppoert mtrr yet
#define ept_pg_set_attr(_e_,_atTr_)                                     \
       ({                                                                   \
       	         (_e_)->raw = _atTr_;                                           \
       	   })

#define ept_set_page_entry(_e_,_attr_,_pfn_)         \
       ({                                                   \
       	      ept_pg_set_attr(_e_,_attr_);                      \
       	      (_e_)->addr = _pfn_;                              \
       	   })
static pml4e_t    __attribute__((aligned(PAGE_SIZE)))  __pml4[PML4E_PER_PML4];
static pdpe_t     __attribute__((aligned(PAGE_SIZE)))  __pdp[PDPE_PER_PDP];
static pde64_t    __attribute__((aligned(PAGE_SIZE)))  __pd[4][PDE64_PER_PD]; /* 4G */
//static pte64_t    __attribute__((aligned(PAGE_SIZE)))  __pt[4][PDE64_PER_PD][PTE64_PER_PT]; /* 4G */
typedef pte64_t (hello_t) [4][PDE64_PER_PD][PTE64_PER_PT] __attribute__((aligned(4096)));
static __page_table_start = 0x400000;
static void setup_page_2M()
{
    uint32_t base, p, n;

    base = 0;
    for(p=0 ; p<4 ; p++)
    {
	base = (p<<PG_1G_SHIFT)>>PG_2M_SHIFT;
	pg_set_entry(&__pdp[p], PG_KRN|PG_RW, page_nr(__pd[p]));

	for(n=0 ; n<PDE64_PER_PD ; n++)
	    pg_set_large_entry(&__pd[p][n], PG_KRN|PG_RW, base+n);
    }
}
void page_map(uint64_t vir_addr, uint64_t phy_addr){
    int pfn = phy_addr >> PG_4K_SHIFT;
    int pfn_pdp = vir_addr>>PG_1G_SHIFT;
    


}
static void setup_page_4k(){
    uint64_t bsp_mem_top = 1*1020*1024*1024;
   // pte64_t *__pt = __page_table_start;

    uint64_t ptaddr = __page_table_start;
    hello_t *__ptp = (hello_t*)0x400000;
    memset(__ptp, 0, sizeof(hello_t));
    size_t pfn = 0;
    for (int i = 0; i < 4; ++i) { // 1G


	//pfn = (p<<PG_1G_SHIFT)>>PG_4K_SHIFT;
	pg_set_entry(&__pdp[i], PG_KRN|PG_RW, page_nr((uint64_t)__pd[i]));
	for (int k = 0; k < PDE64_PER_PD; ++k){ // 2M
	    pg_set_entry(&__pd[i][k], PG_KRN|PG_RW, page_nr((uint64_t)(*__ptp)[i][k]));
	    for(int n = 0 ; n < PTE64_PER_PT ; n++){
		pg_set_entry(&(*__ptp)[i][k][n], PG_KRN|PG_RW, pfn++);
	    }
	}
    }
}
__attribute__((used))void page_init(){
    cr3_reg_t cr3;
    cr4_reg_t cr4;
    /* PML4 first 512G */
    pg_set_entry(&__pml4[0], PG_KRN|PG_RW, page_nr((uint64_t)__pdp));
    /* level-2 PDPE 4x1G */
//#define LARGE_PT
#ifdef LARGE_PT
    setup_page_2M();
#else
    setup_page_4k();
#endif
    cr3.raw = 0UL;
    cr3.pml4.addr = page_nr(__pml4);
//    set_cr(4, get_cr(4)|CR4_PAE|CR4_PSE);
    set_cr(3, cr3.raw);
}


BSP_output_char_function_type BSP_output_char = serial_output_char;
BSP_polling_getchar_function_type BSP_poll_char = serial_poll_char;

void _init(void){
}
void _fini(void){
}
