ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

include_bspdir = $(includedir)/bsp

dist_project_lib_DATA = bsp_specs

include_HEADERS = include/bsp.h
include_HEADERS += ../../shared/include/tm27.h
include_HEADERS += include/usc.h
include_bsp_HEADERS = ../shared/liblnk/regs.h
#isr
include_bsp_HEADERS += ../../shared/include/irq-generic.h
include_bsp_HEADERS += ../../shared/include/irq-info.h
include_bsp_HEADERS += include/irq.h

nodist_include_HEADERS = include/bspopts.h
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h
DISTCLEANFILES = include/bspopts.h

nodist_include_HEADERS += ../../shared/include/coverhd.h

noinst_LIBRARIES = libbspstart.a
libbspstart_a_SOURCES = start/start.S
project_lib_DATA = start.$(OBJEXT)

dist_project_lib_DATA += startup/linkcmds

noinst_LIBRARIES += libbsp.a
libbsp_a_SOURCES =

# startup
libbsp_a_SOURCES += ../../shared/bspclean.c
libbsp_a_SOURCES += ../../shared/bspgetworkarea.c
libbsp_a_SOURCES += ../../shared/bsppredriverhook.c
libbsp_a_SOURCES += startup/bspstart.c
libbsp_a_SOURCES += ../../shared/bootcard.c
libbsp_a_SOURCES += ../../shared/getentropy-cpucounter.c
libbsp_a_SOURCES += ../../shared/sbrk.c
libbsp_a_SOURCES += startup/inittlb.c
libbsp_a_SOURCES += ../shared/startup/idtmem.S
libbsp_a_SOURCES += ../shared/startup/idttlb.S
libbsp_a_SOURCES += ../shared/irq/exception.S
libbsp_a_SOURCES += startup/usc.S
# clock
libbsp_a_SOURCES += clock/ckinit.c
libbsp_a_SOURCES += clock/clock.h

# console
libbsp_a_SOURCES += console/console.c
# liblnk
libbsp_a_SOURCES += ../shared/liblnk/lnklib.S
libbsp_a_SOURCES += ../shared/liblnk/pmon.S
libbsp_a_SOURCES += ../shared/liblnk/regs.h
#isr
libbsp_a_SOURCES += ../../shared/src/irq-generic.c
libbsp_a_SOURCES += ../../shared/src/irq-legacy.c
libbsp_a_SOURCES += ../../shared/src/irq-info.c
libbsp_a_SOURCES += ../../shared/src/irq-shell.c
libbsp_a_SOURCES += ../../shared/src/irq-server.c
libbsp_a_SOURCES += ../shared/irq/vectorexceptions.c
libbsp_a_SOURCES += ../shared/irq/irq.c
libbsp_a_SOURCES += irq/vectorisrs.c
libbsp_a_SOURCES += ../shared/irq/interruptmask.c

libbsp_a_LIBADD  = ../../../libcpu/@RTEMS_CPU@/shared/cache.rel
libbsp_a_LIBADD += ../../../libcpu/@RTEMS_CPU@/shared/interrupts.rel
libbsp_a_LIBADD += ../../../libcpu/@RTEMS_CPU@/rm52xx/timer.rel

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
