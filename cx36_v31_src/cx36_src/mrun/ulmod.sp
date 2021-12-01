*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @ULMOD:  REMINDER OF UNSIGNED DIVIDED BY UNSIGNED FOR IBM SYSTEM/36  *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           REMINDER IN AC0+7                                           *
*                                                                       *
*************************************************************************
*
*  METHOD:   USE @ULDIV WHICH RETURNS REMINDER IN AC0+3
*
@ULMOD   START
*
         EXTRN @AC0
         EXTRN @ULDIV
*
ARR      EQU   8
PC       EQU   X'10'
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+3           (RIGHTMOST BYTE OF UPPER 32 BITS IN AC0)
*
         ST    RETADDR,ARR
         B     @ULDIV
         MVC   ACC(4),ACC_HI    REMINDER INSTEAD OF QUOTIENT TO ACC
         L     RETADDR,PC       RETURN!
*
RETADDR  DS    XL2
         END
