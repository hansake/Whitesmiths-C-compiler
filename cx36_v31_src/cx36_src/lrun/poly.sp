***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   compute polynomial
*
***************************************************
_POLY    START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @CENT
         EXTRN @CRET
         EXTRN @FENTR
         EXTRN @FLEAV
         EXTRN @WR7SA
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
*   DOUBLE _poly(DOUBLE d, DOUBLE *tab, COUNT n)
*
         LA    L30,#XR2
         BD    @CENT                   enter routine
L30      EQU   *
         LA    ARGBIAS(,#XR1),#WR7     WR7 -> argument 0
         LA    L31,#WR6                address of scientific instruction
         B     @FENTR
         B     @CRET                   return from routine
L31      EQU   *
         $XLD  @WR7SA                  argument ptr -> index register
         $DLD  0,I                     d -> work area
         $DST  DWORK
         $HLD  10,I                    n -> binary reg
         $XLD  8,I                     tab -> index register
         $DLD  0,I                     tab[0] -> fp register
         $BIF  L33,L33,L32             if n <= 0 goto end
L32      EQU   *
         $DMLT DWORK                   fp reg *= d
         $XLI  8,I                     next element in tab[]
         $DADD 0,I                     fp reg += tab[++index]
         $HSUB CONST1
         $BIF  L33,L33,L32             if n <= 0 goto end
L33      EQU   *
         $DST  @AC0+0                  return value
         $INVK @FLEAV
*
CONST1   EQU   *
         DC    IL2'1'                  constant 1
DWORK    EQU   *
         DS    FL8                     work area
*
         END
