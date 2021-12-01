***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   scale double exponent
*
***************************************************
_ADDEX   START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @CENT
         EXTRN @CRET
         EXTRN _RANGE
*
ARGBIAS  EQU   250
*
#XR1     EQU   X'01'
#XR2     EQU   X'02'
#ARR     EQU   X'08'
#IAR     EQU   X'10'
#WR4     EQU   X'44'
#WR5     EQU   X'45'
#WR6     EQU   X'46'
#WR7     EQU   X'47'
*
*   DOUBLE _addexp(DOUBLE d, COUNT n, TEXT *msg)
*
         LA    L10,#XR2
         BD    @CENT                   enter routine
L10      EQU   *
         LA    ARGBIAS(,#XR1),#XR2     XR2 -> argument 0 
         CLC   9(2,#XR2),@CONST+3      n == 0 ?
         JNE   L11
         MVC   @AC0+7(8),7(,#XR2)      yes, return d
         J     L19
L11      EQU   *
         MVC   @AC0(1),0(,#XR2)        get exponent
         SBF   @AC0,128                remove sign bit
         MVI   @AC0-1,0
         ALC   @AC0(2),@AC0            exponent <<= 2
         ALC   @AC0(2),@AC0
L12      EQU   *
         TBF   1(,#XR2),128            msb of mantissa != 0 ?
         JF    L13                     yes
         ALC   7(7,#XR2),7(,#XR2)      not yet, shift mantissa left
         SLC   @AC0(2),@CONST+7        and substract one from exponent
         J     L12
L13      EQU   *
         ALC   @AC0(2),9(,#XR2)        exponent += n
L14      EQU   *
         TBF   @AC0,3                  (exponent & 3) == 0 ?
         JT    L15                     yes
         SRC   7(7,#XR2),1             not yet, shift mantissa right
         ALC   @AC0(2),@CONST+7        and add one to exponent
         J     L14
L15      EQU   *
         TBF   @AC0-1,128              negative exponent ?
         JT    L16                     no
         MVC   @AC0+3(4),@CONST+3      yes, underflow return 0
         MVC   @AC0+7(4),@CONST+3
         MVI   @AC0,64                 set exponent bias
         J     L19
L16      EQU   *
         SRC   @AC0(2),2               exponent >>= 2
         CLC   @AC0(2),CONST127        exponent > 127 ?
         JNH   L17                     no
         MVC   245(2,#XR1),11(,#XR2)   overflow, call _range(msg)
         LA    -6,#XR1
         B     _RANGE
         J     L19
L17      EQU   *
         TBF   0(,#XR2),128            negative sign ?
         JT    L18                     no
         SBN   @AC0,128                yes, set result negative
L18      EQU   *
         MVC   @AC0+7(7),7(,#XR2)      copy result to ret. reg.
L19      EQU   *
         B     @CRET                   return to caller
*
CONST127 DC    IL2'127'                constant 127
*
         END
