***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   extract fraction from exponent part
*
***************************************************
_UNPAC   START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @CENT
         EXTRN @CRET
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
*   COUNT _unpack(DOUBLE *pd)
*
         LA    L40,#XR2
         BD    @CENT                   enter routine
L40      EQU   *
         L     ARGBIAS+1(,#XR1),#XR2   XR2 -> *pd
         CLI   1(,#XR2),0              *pd == 0 ?
         JNE   L41
         MVC   @AC0+7(2),@CONST+3      yes, return 0
         J     L44
L41      EQU   *
         MVC   @AC0-1(1),0(,#XR2)      save exponent of *pd
         MVC   @AC0+7(1),0(,#XR2)      get exponent of *pd
         SBF   @AC0+7,128              remove sign bit
         MVI   @AC0+6,0
         SLC   @AC0+7(2),CNST64        and unbias the exponent
         ALC   @AC0+7(2),@AC0+7        exponent <<= 2
         ALC   @AC0+7(2),@AC0+7
L42      EQU   *
         TBF   1(,#XR2),128            msb of mantissa != 0 ?
         JF    L43                     yes
         ALC   7(7,#XR2),7(,#XR2)      not yet, shift mantissa left
         SLC   @AC0+7(2),@CONST+7      and substract one from exponent
         J     L42
L43      EQU   *
         MVC   0(1,#XR2),@AC0-1        get back exponent of *pd
         SBF   0(,#XR2),127            preserve sign bit, zero rest
         ALI   0(,#XR2),64             add exponent bias
L44      EQU   *
         B     @CRET                   return to caller
*
CNST64   DC IL2'64'
*
         END
