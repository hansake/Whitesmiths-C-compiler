*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @LMUL:   MULTIPLY LONG BY LONG FOR IBM SYSTEM/36                     *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RESULT IN AC0                                               *
*                                                                       *
*************************************************************************
*
*  MULTIPLY LONG.   METHOD:
*  ------------------------
*       ACC_HI = 0;
*       FOR (I = 0; I < 32; I++) {
*           IF (ACC & 1) 
*               ACC_HI += M;
*           SHIFT 'ACC_HI + ACC' ONE BIT RIGHT;
*       }
*
*
@LMUL    START
*
         EXTRN @AC0
XR2      EQU   2
ARR      EQU   8
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+3           (RIGHTMOST BYTE OF UPPER 32 BITS IN AC0)
*
         SLC   ACC_HI(4),ACC_HI     CLEAR ACC_HI   
         MVI   COUNT,32             PERFORM 32 LOOP TURNS
*
LOOP     TBN   ACC,1
         JF    L1
         ALC   ACC_HI(4),3(,XR2)
L1       SRC   ACC(8),1             SHIFT 'ACC + ACC_HI' ONE BIT RIGHT
         SLI   COUNT,1
         JNZ   LOOP
*
         B     0(,ARR)              RETURN
*
COUNT    DS    XL1
         END
