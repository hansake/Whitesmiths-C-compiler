 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @FCMP:   Compare float by float for IBM System/36                    *
*                                                                       *
*       left operand in AC0+4                                           *
*       WR7 points to first byte of right operand                       *
*       return value in AC0+4                                           *
*                                                                       *
*************************************************************************
*
@FCMP    START 0
*
#XR1     EQU   X'01'
#XR2     EQU   X'02'
#PSR     EQU   X'04'
#ARR     EQU   X'08'
#IAR     EQU   X'10'
#WR4     EQU   X'44'
#WR5     EQU   X'45'
#WR6     EQU   X'46'
#WR7     EQU   X'47'
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @XR2
*
         EXTRN @FENTR
         EXTRN @FLEAV
         EXTRN @WR7SA
*
         LA    NEXTIN,#WR6         address scientific instruction
         BD    @FENTR
*
NEXTIN   EQU   *
         $XLD  @WR7SA              load index register
         $RLD  @AC0+4              load floating point reg. float prec.
         $RCMP 0,I                 compare, float prec.
         $LSET GT                  test high
         $HST  CCHIGH
         $LSET EQ                  test equal
         $HST  CCEQU
         $LSET LT                  test low
         $HST  CCLOW
         $INVK FIXUP
FIXUP    EQU   *
         SRC   CCEQU+2(2),8        get equal bit (bit 7)
         MVI   CCLOW+2,0
         SRC   CCLOW+2(2),7        get low bit (bit 6)
         ALC   CCEQU+2(1),CCLOW+2
         MVI   CCHIGH+2,0
         SRC   CCHIGH+2(2),6       get high bit (bit 5)
         ALC   CCEQU+2(1),CCHIGH+2
         L     CCEQU+2,#PSR        load into Program Status Register
         BD @FLEAV                 return to caller
*
CCHIGH   EQU   *
         DC    XL3'0'
CCEQU    EQU   *
         DC    XL3'0'
CCLOW    EQU   *
         DC    XL3'0'
*
         END
