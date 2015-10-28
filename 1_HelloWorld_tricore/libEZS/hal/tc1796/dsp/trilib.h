//****************************************************************************
// @Module      Header File
// @Filename    TriLib.h
// @Project     DSP Library for TriCore-1
//----------------------------------------------------------------------------
// @Controller  TriCore V1.2, V1.3
//
// @Compiler    Tasking, GNU
//
// @Version     V1.30
//
// @See Also    -
//
// @Description This is the common header file which is included in the main
//              function of the each example. This header file lists all the 
//              functions of DSP Library. The purpose of its inclusion in
//              main function is to declare the function before calling it.
//              This module contains: 
//              1.Functions prototypes 
//              2.Typedef statements
//              3.Defines
//----------------------------------------------------------------------------
// @Date:       8th April 2004  
//
// @History:    
//              13.08.04
//              Definition of Saturated fract corrected
//              14.12.03
//              The complex number definition has been changed to real||imag
//              format instead of imag||real  format
//              08.08.2003
//              Tasking V2.x (Viper) Support
//              Update workarounds (extended with New Errata names June 2003) 
//              ---------------------------------
//              V1.0 Released on 10 August 2000    

//*****************************************************************************

#ifndef _TRILIB_H_
#define _TRILIB_H_

//******************* Definition of Types *************************************

#define MaxCol 4
#define MaxRow 4

#ifdef  DSPEXT
//================== DSPEXT DEFINITIONS =======================================

#ifdef __CTC__
typedef __wrap __sfract DataS;              // 16-bit data 
typedef __wrap __fract DataL;               // 32-bit data

typedef  __sfract DataS_Sat;    // 16-bit data with saturation 
typedef  __fract  DataL_Sat;     // 32-bit data with saturation

typedef __wrap __sfract __circ* cptrDataS;  //Ptr to Circ-Ptr
typedef __wrap __fract __circ* cptrDataL;   //Ptr to Circ-Ptr


typedef __wrap __sfract __circ  circDataS;  //Circ-Ptr  
typedef __wrap __fract  __circ  circDataL;

// This contains the user defined data types for the complex functions and the 

typedef struct CPLX_16
{
    __wrap __sfract real;
    __wrap __sfract imag;

}CplxS;

typedef struct CPLX_32
{
    __wrap __fract real;
    __wrap __fract imag;

}CplxL;

typedef struct CPLX_32SAT
{
     __fract  real;
     __fract  imag;

}CplxL_Sat;

typedef struct CPLX_16SAT
{
    __wrap __sfract __sat real;
    __wrap __sfract __sat imag;

}CplxS_Sat;

#else //__CTC__

typedef _sfract DataS;              // 16-bit data 
typedef _fract DataL;               // 32-bit data

typedef _sfract _sat DataS_Sat;     // 16-bit data with saturation 
typedef _fract _sat DataL_Sat;      // 32-bit data with saturation

typedef _sfract _circ* cptrDataS;   //Ptr to Circ-Ptr
typedef _fract _circ* cptrDataL;    //Ptr to Circ-Ptr


typedef _sfract _circ  circDataS;   //Circ-Ptr
typedef _fract  _circ  circDataL;

// This contains the user defined data types for the complex functions and the 

typedef struct CPLX_16
{
    _sfract real;
    _sfract imag;

}CplxS;

typedef struct CPLX_32
{
    _fract real;
    _fract imag;

}CplxL;

typedef struct CPLX_32SAT
{
    _fract _sat real;
    _fract _sat imag;

}CplxL_Sat;

typedef struct CPLX_16SAT
{
    _sfract _sat real;
    _sfract _sat imag;

}CplxS_Sat;
  
#endif // __CTC__
#else  // DSPEXT    
//================== NO-DSPEXT DEFINITIONS ====================================

typedef short DataS;                //16-bit data 
typedef short DataS_Sat;            //16-bit data 
typedef int DataL;                  //32-bit data
typedef int DataL_Sat;              //32-bit data with saturation

typedef struct 
{                                      
    DataS* base;
    short index;
    short limit;

}cptrDataS;

typedef struct 
{                                      
    DataL* baseL;
    short  indexL;
    short  limitL;

}cptrDataL;


// This contains the user defined data types for the complex functions and the 

typedef struct
{
    short real;
    short imag;
}CplxS;

typedef struct CPLX_32
{
    int real;
    int imag;

}CplxL;

typedef struct CPLX_32SAT
{
    int real;
    int imag;

}CplxL_Sat;

typedef struct CPLX_16SAT
{
    short real;
    short imag;

}CplxS_Sat;

#endif //DSPEXT

#ifdef _cplusplus
#define _ext_c extern "C"
#else
#define _ext_c extern
#endif

//****************** Complex Number Arithmetic ********************************

//------------------ 16 bit functions prototypes ------------------------------

_ext_c CplxS CplxAdd_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y             // second 1Q15 data
                );

_ext_c CplxS_Sat CplxAdds_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y );          // second 1Q15 data

_ext_c CplxS CplxSub_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y );          // second 1Q15 data

_ext_c CplxS_Sat CplxSubs_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y );          // second 1Q15 data

_ext_c void  CplxMul_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y,            // second 1Q15 data
                CplxL* R);          // address to return the 1Q31 result

_ext_c CplxS_Sat CplxMuls_16(
                CplxS X,            // first 1Q15 data 
                CplxS Y );          // second 1Q15 data

_ext_c CplxS CplxShift_16(
                CplxS X,            // 1Q15 data
                int Y );            // Shift value

_ext_c CplxS CplxConj_16( 
                CplxS X );          // 1Q15 data

_ext_c DataL CplxMag_16( 
                CplxS X);           // First data Pointer

_ext_c DataL CplxPhase_16(
                CplxS X);           // First data Pointer

 
//------------------ 32 bit functions prototypes ------------------------------

_ext_c void CplxAdd_32( 
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL* R );         // Output pointer

_ext_c void CplxAdds_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL_Sat* R );     // Output pointer

_ext_c void CplxSub_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL* R );         // Output pointer

_ext_c void CplxSubs_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL_Sat* R);      // Output pointer

_ext_c void CplxMul_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL* R );         // Output pointer

_ext_c void CplxMuls_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Second data pointer
                CplxL_Sat* R);      // Output pointer

_ext_c void CplxShift_32(
                CplxL* X,           // First data Pointer
                CplxL* Y,           // Output pointer
                int z );            // Shift Value

_ext_c void CplxConj_32( 
                CplxL* X,           // First data Pointer
                CplxL* R);          // Output pointer

_ext_c DataL CplxMag_32( 
                CplxL* X);          // First data Pointer

_ext_c DataL CplxPhase_32( 
                CplxL* X);          // First data Pointer
                        
                                           
//***************************** Vector Arithmetic *****************************

_ext_c DataL VecDotPro(
                DataS* X,           //First input vector 
                DataS* Y,           //Second input vector
                int nX);            //Vector length

_ext_c int  VecMaxIdx(
                DataS* X,           //Input vector
                int nX);            //Vector length

_ext_c int  VecMinIdx(
                DataS* X,           //Input vector
                int nX);            //Vector length

_ext_c DataS VecMaxVal(
                DataS* X,           //Input vector
                int nX);            //Vector length

_ext_c DataS VecMinVal(
                DataS* X,           //Input vector
                int nX);            //Vector length


_ext_c void VecAdd(
                DataS* X,           //Input vector1
                DataS* Y,           //Input vector2
                DataS_Sat* R,       //Output vector
                int nX);            //Vector length

_ext_c void VecSub(
                DataS* X,           //Input vector1
                DataS* Y,           //Input vector2
                DataS_Sat* R,       //Rput vector
                int nX);            //Vector length

//***************************** FIR Filters ***********************************

_ext_c  DataS Fir_4_16(
                DataS       X,      //Real Input value
                DataS*      H,      //Ptr to Coeff_Buffer
                cptrDataS*  DLY     //Ptr to Circ-Ptr of Delay-Buffer
                );

_ext_c  void FirBlk_4_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                cptrDataS   H,      //Circ-Ptr of Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX      //Size of Input-Buffer
                );
                
_ext_c DataS Fir_16(
                DataS       X,      //Real Input value
                DataS*      H,      //Ptr to Coeff_Buffer
                cptrDataS*  DLY     //Ptr to Circ-Ptr of Delay-Buffer
                );

_ext_c void FirBlk_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                cptrDataS   H,      //Circ-Ptr of Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX      //Size of Input-Buffer
                );

_ext_c DataS FirSym_4_16(
                DataS       X,      //Real Input value
                DataS*      H,      //Ptr to Coeff_Buffer
                cptrDataS*  DLY     //Ptr to Circ-Ptr of Delay-Buffer
                );
 
_ext_c void FirSymBlk_4_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                DataS*      H,      //Ptr to Circ-Ptr of Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX      //Size of Input-Buffer
                );

_ext_c DataS FirSym_16(
                DataS       X,      //Real Input value
                DataS*      H,      //Ptr to Coeff_Buffer
                cptrDataS*  DLY     //Ptr to Circ-Ptr of Delay-Buffer
                );

_ext_c void FirSymBlk_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                DataS*      H,      //Ptr to Circ-Ptr of Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX      //Size of Input-Buffer
                );

_ext_c void FirDec_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                cptrDataS   H,      //Circ-Ptr of Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX,     //Size of Input-Buffer
                int         Df      //Decimation factor
                );

 _ext_c void FirInter_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                DataS*      H,      //Ptr to Output-Buffer    
                cptrDataS* DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                int         nX,     //Size of Input-Buffer
                int         If      //Interpolation factor
                );

//***************************** IIR Filters ***********************************

_ext_c  DataS IirBiq_4_16(
                DataS       X,      //Real Input value
                short*      H,      //Ptr to Coeff_Buffer
                DataS*      DLY,    //Ptr to Delay-Buffer       
                int         nBiq    //Number of BiQuads
                );

_ext_c void IirBiqBlk_4_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                short*      H,      //Ptr to Coeff_Buffer
                DataS*      DLY,    //Ptr to Delay-Buffer       
                int         nBiq,   //Number of BiQuads
                int         nX      //Size of Input-Buffer
                );

 _ext_c DataS IirBiq_5_16(
                DataS       X,      //Real Input value
                short*      H,      //Ptr to Coeff_Buffer
                DataS*      DLY,    //Ptr to Delay-Buffer       
                int         nBiq    //Number of BiQuads
                );

 _ext_c void IirBiqBlk_5_16(
                DataS*      X,      //Ptr to Input-Buffer 
                DataS*      R,      //Ptr to Output-Buffer 
                short*      H,      //Ptr to Coeff_Buffer
                DataS*      DLY,    //Ptr to Delay-Buffer       
                int         nBiq,   //Number of BiQuads
                int         nX      //Size of Input-Buffer
                );

//***************************** Adaptive Filters ******************************

_ext_c DataS  Dlms_4_16(
                DataS       X,      //Real Input value
                DataS*      H,      //Ptr to Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                DataS       D,      //Real Expected output value
                DataS*      Err,    //Ptr to Error
                DataS       U       //Step Size
                );

_ext_c void     DlmsBlk_4_16(
                DataS*       X,     //Ptr to Input-Buffer
                DataS*       R,     //Ptr to Output-Buffer
                cptrDataS    H,     //Ptr to Circ-Ptr of Coeff-Buffer
                cptrDataS*   DLY,   //Ptr to Circ-Ptr of Delay-Buffer
                int          nX,    //Size of Input-Bufffer
                DataS*       D,     //Ptr to Desired-Output-Buffer
                DataS*       Err,   //Ptr to Error 
                DataS        U      //Step Size
                );

_ext_c DataL   Dlms_2_16x32(
                DataS       X,      //Real Input value
                DataL*      H,      //Ptr to Coeff-Buffer
                cptrDataS*  DLY,    //Ptr to Circ-Ptr of Delay-Buffer
                DataL       D,      //Real Expected output value
                DataL*      Err,    //Ptr to Error
                DataL       U       //Step Size
                );

_ext_c void     DlmsBlk_2_16x32(
                DataS*       X,     //Ptr to Input-Buffer
                DataL*       R,     //Ptr to Output-Buffer
                cptrDataL    H,     //Ptr to Circ-Ptr of Coeff-Buffer
                cptrDataS*   DLY,   //Ptr to Circ-Ptr of Delay-Buffer
                int          nX,    //Size of Input-Bufffer
                DataL*       D,     //Ptr to Desired-Output-Buffer
                DataL*       Err,   //Ptr to Error 
                DataL        U      //Step Size
                );

_ext_c CplxS CplxDlms_4_16( 
                CplxS       X,      //Complex Input value
                DataS*      H,      //Ptr to Coeff-Buffer
                cptrDataS*  DLYr,   //Ptr to Circ-Ptr of Delay-Buffer (real)
                cptrDataS*  DLYi,   //Ptr to Circ-Ptr of Delay-Buffer (imag)
                CplxS       D,      //Ptr to Desired-Output-Buffer
                CplxS*      Err,    //Ptr to comples Error 
                DataS       U       //Step Size     
                );

_ext_c void CplxDlmsBlk_4_16( 
                CplxS*      X,      //Ptr to Complex Input-Buffer
                CplxS*      R,      //Ptr to Complex Output-Buffer
                DataS*      H,      //Ptr to Coeff-Buffer
                cptrDataS*  DLYr,   //Ptr to Circ-Ptr of Delay-Buffer (real)
                cptrDataS*  DLYi,   //Ptr to Circ-Ptr of Delay-Buffer (imag)
                int         nX,     //Input-Buffer size
                CplxS*      D,      //Ptr to Desired-Output-Buffer
                CplxS*      Err,    //Ptr to complex Error
                DataS       U       //Step Size     
                );

//***************************** Transforms ************************************

_ext_c short FFT_2_16(
                CplxS*       R,     //Ptr to output-Buffer
                CplxS*       X,     //Ptr to Input-Buffer   
                CplxS*       TF,    //Ptr to Twiddle-Factor table
                int          nX     //Size of Input-Buffer
                );

_ext_c short FFTReal_2_16(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer         
                );

_ext_c short IFFT_2_16(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer         
                );

_ext_c short IFFTReal_2_16(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX,    //Size of Input-Buffer         
                int          SFlg   //Flag if TRUE input is scaled down by 2
                );

_ext_c CplxS* FFT_TF_16(
                CplxS*      TF,     //Ptr to Twiddle Factor
                CplxS*      Wn,     //Ptr to Temp Buffer
                int         nX      //Size of Input-Buffer
                );

_ext_c CplxS* FFT_CnvrtWn_16(
                CplxS*      TF,     //Ptr to twiddle factor
                CplxS*      Wn,     //Ptr to Temp Buffer
                int         nX      //Size of Input-Buffer
                );

_ext_c short FFT_2_32(
                CplxL*       R,     //Ptr to output-Buffer
                CplxL*       X,     //Ptr to Input-Buffer   
                CplxL*       TF,    //Ptr to Twiddle-Factor table
                int          nX     //Size of Input-Buffer
                );

_ext_c short FFTReal_2_32(
                CplxL*       R,     //Ptr to output-Buffer         
                CplxL*       X,     //Ptr to Input-Buffer          
                CplxL*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer         
                );

_ext_c short IFFT_2_32(
                CplxL*       R,     //Ptr to output-Buffer         
                CplxL*       X,     //Ptr to Input-Buffer          
                CplxL*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer         
                );

_ext_c short IFFTReal_2_32(
                CplxL*       R,     //Ptr to output-Buffer         
                CplxL*       X,     //Ptr to Input-Buffer          
                CplxL*       TF,    //Ptr to Twiddle-Factor table  
                int          nX,    //Size of Input-Buffer         
                int          SFlg   //Flag if TRUE input is scaled down by 2
                );

_ext_c CplxL* FFT_TF_32(
                CplxL*      TF,     //Ptr to Twiddle-Factor table  
                int         nX      //Size of Input-Buffer
                );

_ext_c short FFT_2_16X32(
                CplxS*       R,     //Ptr to output-Buffer
                CplxS*       X,     //Ptr to Input-Buffer   
                CplxS*       TF,    //Ptr to Twiddle-Factor table
                int          nX     //Size of Input-Buffer
                );

_ext_c short FFTReal_2_16X32(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer         
                );

_ext_c short IFFT_2_16X32(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX     //Size of Input-Buffer
                );

_ext_c short IFFTReal_2_16X32(
                CplxS*       R,     //Ptr to output-Buffer         
                CplxS*       X,     //Ptr to Input-Buffer          
                CplxS*       TF,    //Ptr to Twiddle-Factor table  
                int          nX,    //Size of Input-Buffer         
                int          SFlg   //Flag if TRUE input is scaled down by 2
                );

_ext_c CplxS* FFT_TF_16X32(
                CplxS*      TF,     
                int         nX      //Size of Input-Buffer
                );


_ext_c DataS* DCT_2_8(
                DataS*      X       //Ptr. to two dimensional (8X8) spatial 
                );                  //block

_ext_c DataS* IDCT_2_8(
                DataS*      X       //Ptr. to two dimensional (8X8) spatial 
                );                  //block
                

//***************************** Mathematical **********************************

_ext_c short Arctan_32(
                int  X              //Real Input value       
                );                                           
                                                             
_ext_c DataS Sine_32(                                        
                int  X              //Real Input value       
                );                                           
                                                             
_ext_c DataS Cos_32(                                         
                int  X              //Real Input value       
                );                                           
                                                             
_ext_c short Expn_16(                                        
                DataS X             //Real Input value       
                );                                           
                                                             
_ext_c short Sqrt_32(                                        
                int X               //Real Input value       
                );                                           
                                                             
_ext_c short Ln_32(                                          
                int X               //Real Input value       
                );                                           
                                                             
_ext_c int AntiLn_16(                                        
                short X             //Real Input value       
                );  
                
_ext_c int XpowY_32(                                         
                int X,              //Real Input value 
                DataS Y             //Real Exponent value       
                );  

_ext_c void RandInit_16(void); 
                
_ext_c void Rand_16(
                int nX,             //Size of Output-Vector
                short *R            //Ptr to Output-Vector
                );
                                                             
//***************************** Statistical ***********************************

_ext_c DataL Avg_16(
                DataS    *X,        //Ptr to Input-Buffer
                int      nX         //Size of Input-Buffer
                ); 

_ext_c void ACorr_16(
                DataS   *X,         //Ptr to Input-Buffer   
                DataL   *R,         //Ptr to Output-Buffer
                int     nX,         //Size of Input-Buffer
                int     nR          //Size of Output-Buffer
                );

_ext_c void Conv_16(
                DataS   *X,         //Ptr to first Input-Buffer      
                DataS   *H,         //Ptr to second Input-Buffer  
                DataL   *R,         //Ptr to Output-Buffer
                int     nR,         //Size of Output-Buffer
                int     nH          //Size of second Input-Buffer
                );

//***************************** Matrix ****************************************

_ext_c void MatAdd_16(             
                short   X[][MaxCol],//Input Matrix       
                short   Y[][MaxCol],//Input Matrix       
                short   R[][MaxCol],//Output Matrix      
                int     nRow,       //Number of rows     
                int     nCol        //Number of columns  
                );                                       
                                                         
_ext_c void MatSub_16(                                   
                short   X[][MaxCol],//Input Matrix       
                short   Y[][MaxCol],//Input Matrix       
                short   R[][MaxCol],//Output Matrix      
                int     nRow,       //Number of rows     
                int     nCol        //Number of columns  
                );                                       
                                                         
_ext_c void MatTrans_16(                                 
                short X[][MaxCol],  //Input Matrix       
                short R[][MaxRow],  //Output Matrix      
                int nRow,           //Number of rows     
                int nCol            //Number of columns  
                );                                       

_ext_c void MatMult_16(                              
                DataS X[][MaxCol],  //Input Matrix  
                DataS Y[][MaxCol],  //Input Matrix 
                DataS R[][MaxCol],  //Output Matrix      
                int nRowX,          //Number of rows of first matrix     
                int nColX,          //Number of columns of first matrix  
                int nColY           //Number of columns of second matrix
                );                                 

//*****************************************************************************

#endif // End _TRILIB_H_

//------------------- END OF FILE ---------------------------------------------

