include $(top_srcdir)/automake/compile.am

include_rtemsdir = $(includedir)/rtems
include_rtems_HEADERS = rtems/asm.h

include_rtems_scoredir = $(includedir)/rtems/score
include_rtems_score_HEADERS = rtems/score/cpu.h
include_rtems_score_HEADERS += rtems/score/cpuimpl.h
include_rtems_score_HEADERS += rtems/score/x86_64.h
include_rtems_score_HEADERS += rtems/score/cpu_asm.h
include_rtems_score_HEADERS += rtems/score/types.h
include_rtems_score_HEADERS += rtems/score/cpuatomic.h

noinst_LIBRARIES = libscorecpu.a
libscorecpu_a_SOURCES = cpu.c cpu_asm.c cpu_asm2.S
libscorecpu_a_SOURCES += cpucounterread.c
libscorecpu_a_CPPFLAGS = $(AM_CPPFLAGS)

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
