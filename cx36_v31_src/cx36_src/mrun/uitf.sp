 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @UITF:   Unsigned integer to float conversion for IBM System/36      *
*                                                                       *
*       WR7 points to first byte of right operand                       *
*       return value in AC0+4                                           *
*                                                                       *
*************************************************************************
*
@UITF    START 0
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
         EXTRN @FESIS
         EXTRN @FESAV
         EXTRN @FLEAV
         EXTRN @WR7SA
*
         LA    TOUNSN,#WR6         address to return to
         BD    @FESAV              save registers
TOUNSN   EQU   *
         MVI   @AC0+4,0
         MVI   @AC0+5,0
         MVC   @AC0+7(2),1(,#XR2)
*
         LA    NEXTIN,#WR6         address scientific instruction
         BD    @FESIS
NEXTIN   EQU   *
         $BLD  @AC0+4              load binary register
         $BCNR                     binary to float conversion
         $RST  @AC0+4              store floating point reg. float prec.
         $INVK @FLEAV              return to caller
*
         END
