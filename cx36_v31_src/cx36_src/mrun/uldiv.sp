*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @ULDIV:  DIVIDE UNSIGNED LONG BY UNSIGNED LONG FOR IBM SYSTEM/36     *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           QUOTIENT IN AC0+7, REMINDER IN AC0+3                        *
*                                                                       *
*************************************************************************
*
*  METHOD:
*  -------
*    FOR (I = 0; I < 32; I++) {
*       SHIFT 'ACC_HI + ACC' ONE BIT LEFT;
*       ACC_HI -= ARG2;
*       IF (OVERFLOW)
*           ACC_HI += ARG2;
*       ELSE
*           ACC += 1;
*    }
*
@ULDIV   START
         EXTRN @AC0
*
XR2      EQU   2
ARR      EQU   8
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+3           (RIGHTMOST BYTE OF UPPER 32 BITS IN AC0)
*
*
         SLC   ACC_HI(4),ACC_HI     CLEAR ACC_HI   
         MVI   COUNT,32             PERFORM 32 LOOP TURNS
*
LOOP     ALC   ACC(8),ACC           SHIFT 'ACC + ACC_HI' ONE BIT LEFT
         SLC   ACC_HI(4),3(,XR2)
         JL    L1
         ALI   ACC,1
L2       SLI   COUNT,1
         JNZ   LOOP
         B     0(,ARR)              RETURN
L1       ALC   ACC_HI(4),3(,XR2)
         J     L2
*
COUNT    DS    XL1
         END
