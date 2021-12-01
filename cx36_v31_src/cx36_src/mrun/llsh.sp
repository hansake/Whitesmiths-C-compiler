*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @LLSH:   LEFT SHIFT LONG BY INTEGER COUNT                            *
*           FOR IBM SYSTEM/36                                           *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
@LLSH    START
*
         EXTRN @AC0
XR2      EQU   2
ARR      EQU   8
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF @AC0)
*CNT     EQU   1(2,XR2)         (RIGHTMOST BYTE OF SHIFT COUNT (INT))
*
*
         CLC   1(2,XR2),ZERO    RETURN IF COUNT == 0  (16 BIT NUMBER)
         BNH   0(,ARR)
         CLC   1(2,XR2),N32     MAKE SURE COUNT < 32
         JNH   L1
         SLC   ACC(4),ACC       RESULT IS ZERO
         B     0(,ARR)          RETURN!
L1       MVC   CNT,1(,XR2)
*
LOOP     ALC   ACC(4),ACC       SHIFT ONE STEP LEFT
         SLI   CNT,1
         JNZ   LOOP             MORE STEPS TO SHIFT
         B     0(,ARR)          RETURN!
*
*****
*
CNT      DS    XL1
ZERO     DC    IL2'0'
N32      DC    IL2'32'
         END
