*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @UIDIV:  DIVIDE UNSIGNED BY UNSIGNED FOR IBM SYSTEM/36               *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           QUOTIENT IN AC0+7, REMINDER IN AC0+5                        *
*                                                                       *
*************************************************************************
*
*  METHOD:
*  -------
*    FOR (I = 0; I < 16; I++) {
*       SHIFT 'ACC_HI + ACC' ONE BIT LEFT;
*       ACC_HI -= ARG2;
*       IF (OVERFLOW)
*           ACC_HI += ARG2;
*       ELSE
*           ACC += 1;
*    }
*
@UIDIV   START
*
         EXTRN @AC0
XR2      EQU   2
ARR      EQU   8
ACC      EQU   @AC0+7               (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+5               (RIGHTMOST BYTE OF UPPER 16 BITS IN AC0)
*
         SLC   ACC_HI(2),ACC_HI     CLEAR ACC_HI   
         MVI   COUNT,16             PERFORM 16 LOOP TURNS
*
LOOP     ALC   ACC(4),ACC           SHIFT 'ACC + ACC_HI' ONE BIT LEFT
         SLC   ACC_HI(2),1(,XR2)
         JL    L1
         ALI   ACC,1
L2       SLI   COUNT,1
         JNZ   LOOP
         B     0(,ARR)              RETURN
L1       ALC   ACC_HI(2),1(,XR2)
         J     L2
*
COUNT    DS    XL1
         END
