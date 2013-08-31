/* md_types.h
 
   Defines types
*/
#ifndef __MD_TYPES_H__
#define __MD_TYPES_H__

#include <stdint.h>
#include <string.h>

// data types
typedef uint32_t u32 ;
typedef int32_t  s32 ;
typedef uint16_t u16 ;
typedef int16_t  s16 ;
typedef uint8_t  u8  ;
typedef int8_t   s8  ;
typedef uint8_t  bool_t;


#define U8_MAX  ( (u8)0xFF )
#define U16_MAX ( (u16)0xFFFF )
#define U32_MAX ( (u32)0xFFFFFFFF )

#define S8_MAX  ( (s8)0x7F )
#define S16_MAX ( (s16)0x7FFF )
#define S32_MAX ( (s32)0x7FFFFFFF )


// functions 
#define pa_strcmp strcmp


// Return type of a PAOS function
typedef s16 PA_RET;
// Return value of a PAOS function ( on success )
#define PA_OK    (0)
// Return value of a PAOS function ( on failure )
#define PA_ERROR (-1)


// Instruction Barrier 
// Do not allow CPU to exchange order of instructions 
// above PA_INST_BARRIER and ones below PA_INST_BARRIER
#define PA_INST_BARRIER


// How to Treat Inline
#define PA_INLINE static inline


#endif /* __MD_TYPES_H__ */
