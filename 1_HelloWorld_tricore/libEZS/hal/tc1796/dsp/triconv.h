//****************************************************************************** 
// @Module:     Include file 
// @Filename:   TriConv.h
// @Project:    DSP library for TriCore-1 V1.2, V1.3
//------------------------------------------------------------------------------
// @Controller: TriCore V1.2, V1.3
//
// @Compiler:   GNU
//
// @Version:    V1.0
//
// @Description:This file includes macros defining the C calling conventions 
//              and the system  level constants. This file should be included 
//              in all the assembly source code files. It works as the
//              header file.
//
// @See Also:   None
//              
// @References: 1. <TriCore Embedded Applications Binary Interface V1.0>
//              2. <TriCore Architecture Manual V1.2.1>
//              3. <Infineon Tricore Optimization Guide V1.2>       
//
// @Date:       
//
// @History:    
//              WA compliant with GNU and TriLib V1.30
//			   -------------------------------------------------------
//              WA compliant with GNU and TriLib V1.21
//              V1.0 Released on 29th May 2000    
//
//******************************************************************************

#ifndef _TRICONV_H_
#define _TRICONV_H_
#endif
//------------------- Universal Definitions------------------------------------
    
    .equ FALSE       ,      0
    .equ TRUE        ,      1
    .equ SIZE_SHORT  ,      2
    .equ SIZE_SFRACT ,      2
    .equ SIZE_CMPLX  ,      4
    .equ SIZE_CMPLX_32   ,  8
    .equ MAX_W16     ,      0X7FFF
    .equ MIN_W16     ,      0x8000

//------------------- Register Definitions--------------------------------------
    
    .equ    PSW  ,      0xFE04
    .equ    PCXI,       0xFE00
    .equ    PC  ,       0xFE08
    .equ    FCX ,       0xFE38
    .equ    LCX ,       0xFE3C
    .equ    ISP ,       0xFE28
    .equ    ICR ,       0xFE2C
    .equ    BIV     ,   0xFE20
    .equ    BTV     ,   0xFE24
    .equ    SYSCON  ,   0xFE14
    .equ    DBGSR   ,   0xFD00
    .equ    EXEVT   ,   0xFD08
    .equ    SWEVT   ,   0xFD10
    .equ    CREVT   ,   0xFD0C
    .equ    TR0EVT  ,   0xFD20
    .equ    TR1EVT  ,   0xFD24  
    .equ    GPRWB   ,   0xFD04
    .equ    RSTSR   ,   0xFE0C
    .equ    RSTREQ  ,   0xFE10
    .equ    DVV0    ,   0xFD80
    .equ    DVM0    ,   0xFD84
    .equ    DVV1    ,   0xFD88
    .equ    DVM1    ,   0xFD8C
                        
//------------------- Program Status Bit Definition-----------------------------

    .equ    C       ,   31
    .equ    V       ,   30
    .equ    SV      ,   29
    .equ    AV      ,   28
    .equ    SAV     ,   27
    .equ    IS      ,   9
    .equ    GW      ,   8
    .equ    CDE     ,   7

//------------------- Constant Definitions for pointer offset-------------------            
        
    .equ        W16,         2      
    .equ        W32,         4          
    .equ        W64,         8      


//------------------------------------------------------------------------------
//       Silicon Workarounds Definitions
//------------------------------------------------------------------------------


//-------------------------------------------------
//
// TC-1.2 definitions
//
//-------------------------------------------------
// Internal Definition: _TC112_COR1
// User Definition TriLib Style: COR1
// User Definition Tool Style: mcpu10

#if COR1
#define TRWA_CPU_TC018 1
#endif

//-------------------------------------------------
// Internal Definition: _TC112_COR10
// User Definition TriLib Style: COR10
// User Definition Tool Style: ---
#if COR10
#define TRWA_CPU_TC027  1
#endif

//-------------------------------------------------
// Internal Definition: _TC112_COR14
// User Definition TriLib Style: COR14
// User Definition Tool Style: mcpu8
#if COR14
#define TRWA_CPU_TC031  1
#endif


#if ERRATA_CPU10
#define TRWA_CPU_TC018   1
#define (CPU TC.018)	 1
#endif

#if ERRATA_CPU8
#define TRWA_CPU_TC031  1
#define  (CPU TC.031)   1
#endif

//-------------------------------------------------
//
// TC-1.3 definitions
//
//-------------------------------------------------
// Internal Definition: _TC113_CPU5
// User Definition TriLib Style: CPU5
// User Definition Tool Style: ---
#if CPU5
#define TRWA_CPU_TC113_CPU5   1
#endif

//-------------------------------------------------
// Internal Definition: _TC113_CPU9
// User Definition TriLib Style: ---
// User Definition Tool Style: mcpu9
#if CPU9
#define TRWA_CPU_TC034   1
#endif

//-------------------------------------------------
// Internal Definition: _TC113_CPU13
// User Definition TriLib Style: ---
// User Definition Tool Style: mcpu13
#ifdef CPU13
#define TRWA_CPU_TC113_CPU14  1
#endif

//------------------- System Level Macros--------------------------------------

//-----------------------------------------------------------------------------
// Macro:       FEnter
//
// DESCRIPTION: Setup a local frame for C functions.
//              Space indicates the stack space reserved.
//
//-----------------------------------------------------------------------------

.macro          FEnter  Space
                #if TRWA_CPU_TC113_CPU14
                dsync
                #if TRWA_CPU_TC034
                nop
                nop
                #endif
                #endif

                 .if \Space == 0
                 .else
                    sub.a   %sp,\Space
                .endif
                .endm

//-----------------------------------------------------------------------------
// Macro:       FReturn
//
// DESCRIPTION: Return from a C function. 
//
//-----------------------------------------------------------------------------

.macro          FReturn         
                    ret
                .endm

//-----------------------------------------------------------------------------
// Macro:       ClearRpair
//
// DESCRIPTION: It clears register pair to zero
//
//-----------------------------------------------------------------------------

.macro  ClearRpair  Rpair
             mul    \Rpair,%D0,0
            .endm
        



//------------------- END OF FILE ---------------------------------------------