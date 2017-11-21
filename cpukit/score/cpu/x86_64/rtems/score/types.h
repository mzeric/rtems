/**
 * @file
 * 
 * @brief No-CPU Type Definitions
 * 
 * This include file contains type definitions pertaining to the Intel
 * no_cpu processor family. 
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_TYPES_H
#define _RTEMS_SCORE_TYPES_H

#include <rtems/score/basedefs.h>

#ifndef ASM

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This section defines the basic types for this processor.
 */

/** Type that can store a 32-bit integer or a pointer. */
typedef uintptr_t CPU_Uint32ptr;

typedef union {
   uint16_t raw;
   int16_t sraw;
   uint8_t  byte[2];

   /* H/L byte access */
   struct
   {
      uint8_t blow;
      uint8_t bhigh;

   }  RTEMS_PACKED;

}  RTEMS_PACKED raw16_t;
typedef union {
   uint32_t raw;
   int32_t sraw;
   uint8_t  byte[4];

   /* Full decomposition */
   struct
   {
      raw16_t _wlow;
      raw16_t _whigh;

   }  RTEMS_PACKED;

   /* Fast acces to lower parts */
   struct
   {
      union
      {
         struct
         {
            uint8_t blow;
            uint8_t bhigh;

         }	RTEMS_PACKED;

         uint16_t wlow;

      }	 RTEMS_PACKED;

      uint16_t whigh;

   }  RTEMS_PACKED;

}	RTEMS_PACKED raw32_t;

typedef union {
   uint64_t raw;
   int64_t sraw;
   uint8_t  byte[8];

   /* Full decomposition */
   struct
   {
      raw32_t _low;
      raw32_t _high;

   }  RTEMS_PACKED;

   /* Fast access to lower parts */
   struct
   {
      union
      {
         struct
         {
            union
            {
               struct
               {
                  uint8_t blow;
                  uint8_t bhigh;

               }  RTEMS_PACKED;

               uint16_t wlow;

            }  RTEMS_PACKED;

            uint16_t whigh;

         }  RTEMS_PACKED;

         uint32_t  low;

      }  RTEMS_PACKED;

      uint32_t  high;

   }  RTEMS_PACKED;

} RTEMS_PACKED raw64_t;



#ifdef __cplusplus
}
#endif

#endif  /* !ASM */

#endif
