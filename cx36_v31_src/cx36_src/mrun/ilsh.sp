*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @ILSH:   LEFT SHIFT INTEGER BY INTEGER COUNT FOR IBM SYSTEM/36       *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*
@ILSH    START
*
         EXTRN @AC0
XR2      EQU   2
ARR      EQU   8
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF @AC0)
*
*
         CLC   1(2,XR2),ZERO    RETURN IF COUNT == 0  (16 BIT NUMBER)
         BNH   0(,ARR)
         CLC   1(2,XR2),N16     MAKE SURE COUNT < 16
         JNH   L1
         SLC   ACC(2),ACC       RESULT IS ZERO
         B     0(,ARR)          RETURN!
L1       MVC   CNT,1(,XR2)
*
LOOP     ALC   ACC(2),ACC       SHIFT ONE STEP LEFT
         SLI   CNT,1
         JNZ   LOOP             MORE STEPS TO SHIFT
         B     0(,ARR)          RETURN!
*
*****
*
CNT      DS    XL1
ZERO     DC    IL2'0'
N16      DC    IL2'16'
         END
