/**
 * @file
 *
 * @brief CPU Port Implementation API
 */

/*
 * Copyright (c) 2013, 2016 embedded brains GmbH
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_CPUIMPL_H
#define _RTEMS_SCORE_CPUIMPL_H

#include <rtems/score/cpu.h>

/**
 * @brief The size of the CPU specific per-CPU control.
 *
 * This define must be visible to assember files since it is used to derive
 * structure offsets.
 */
#define CPU_PER_CPU_CONTROL_SIZE 0

#ifndef ASM

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* ASM */

#endif /* _RTEMS_SCORE_CPUIMPL_H */
