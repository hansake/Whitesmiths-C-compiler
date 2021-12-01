 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @DNEG:   Negate double for IBM System/36                             *
*                                                                       *
*       left operand in AC0+0                                           *
*       return value in AC0+0                                           *
*                                                                       *
*************************************************************************
*
@DNEG    START 0
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
         $DLD  @AC0+0              load floating point reg. double prec.
         $RSGN                     change sign, double prec.
         $DST  @AC0+0              store floating point reg. double prec.
         $INVK @FLEAV              return to caller
*
         END
