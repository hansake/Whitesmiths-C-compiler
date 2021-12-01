 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @FSUB:   Substract float by float for IBM System/36                  *
*                                                                       *
*       left operand in AC0+4                                           *
*       WR7 points to first byte of right operand                       *
*       return value in AC0                                             *
*                                                                       *
*************************************************************************
*
@FSUB    START 0
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
         $RSUB 0,I                 substract, float prec.
         $RST  @AC0+4              store floating point reg. float prec.
         $INVK @FLEAV              return to caller
*
         END
