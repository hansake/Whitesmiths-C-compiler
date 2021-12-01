 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @ULTF:   Unsigned long to float conversion for IBM System/36         *
*                                                                       *
*       WR7 points to first byte of right operand                       *
*       return value in AC0+0                                           *
*                                                                       *
*************************************************************************
*
@ULTF    START 0
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
         MVC   @AC0+7(4),3(,#XR2)
         TBN   @AC0+4,X'80'        is it big
         JF    T1                  no
         SLI   @AC0+4,X'80'        else, make it not so very big
         LA    HIBIAS,#WR7         and address bias to fix it
         J     T2
T1       EQU   *
         LA    NOBIAS,#WR7         address bias zero
T2       EQU   *
*
         LA    NEXTIN,#WR6         address scientific instruction
         BD    @FESIS
*
NEXTIN   EQU   *
         $XLD  @WR7SA              load index register
         $BLD  @AC0+4              load binary register, long
         $BCNR                     binary to float conversion
         $RADD 0,I                 add bias
         $RST  @AC0+4              store floating point reg. float prec.
         $INVK @FLEAV              return to caller
*
NOBIAS   EQU   *
         DC    FL4'0'
HIBIAS   EQU   *
         DC    FL4'2147483648'
         END
