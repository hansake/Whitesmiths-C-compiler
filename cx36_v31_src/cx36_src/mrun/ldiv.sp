*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @LDIV:   DIVIDE LONG BY LONG FOR IBM SYSTEM/36                       *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           QUOTIENT IN AC0+7, REMINDER IN AC0+3                        *
*                                                                       *
*************************************************************************
*
*  METHOD:  USE THE UNSIGNED DIVIDE ROUTINES
*
@LDIV    START
*
         EXTRN @AC0
         EXTRN @ULDIV
*
XR2      EQU   2
ARR      EQU   8
PC       EQU   X'10'
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF LEFT OPERAND AND RESULT)
ACCL     EQU   @AC0+4           (LEFTMOST BYTE OF LEFT OPERAND)
ACC_R    EQU   @AC0+3           (RIGHTMOST BYTE OF REMINDER)
*
         ST    RETADDR,ARR
         MVI   SIGN,0
         TBN   ACCL,X'80'       TEST SIGN OF LEFT
         JF    POS
         ALI   SIGN,1           NEGATIVE!
         SLC   TMP,TMP          COMPLEMENT ACC
         SLC   TMP,ACC
         MVC   ACC(4),TMP
*
POS      MVC   XR2SAVE,3(,XR2)
         TBN   0(,XR2),X'80'    TEST SIGN OF RIGHT
         JF    POS2
         ALI   SIGN,1           NEGATIVE!
         SLC   TMP,TMP          COMPLEMENT RIGHT OPERAND
         SLC   TMP,3(,XR2)
         MVC   3(4,XR2),TMP
*
POS2     B     @ULDIV           DIVIDE!
         TBN   SIGN,1
         JF    RETURN           RESULT SHOULD BE POSITIVE
         SLC   TMP,TMP          COMPLEMENT QUOTIENT
         SLC   TMP,ACC
         MVC   ACC(4),TMP
         SLC   TMP,TMP          COMPLEMENT REST
         SLC   TMP,ACC_R
         MVC   ACC_R(4),TMP
*
RETURN   MVC   3(4,XR2),XR2SAVE
         L     RETADDR,PC
*
XR2SAVE  DS    XL4
RETADDR  DS    XL2
TMP      DS    XL4
COUNT    DS    XL1
SIGN     DS    XL1
         END
