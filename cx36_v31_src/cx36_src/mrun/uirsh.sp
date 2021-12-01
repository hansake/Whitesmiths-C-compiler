*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @UIRSH:  UNSIGNED RIGHT SHIFT BY INTEGER COUNT                       *
*           FOR IBM SYSTEM/36                                           *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*
@UIRSH   START
         ENTRY @RSH16
         EXTRN @AC0
*
XR2      EQU   2
ARR      EQU   8
PC       EQU   X'10'
ACC      EQU   @AC0+7
ACC_SGN  EQU   @AC0+5
*
         MVC   ACC_SGN(2),ZERO          NO SIGN EXTENSION
*
************
*
*  INTEGER RIGHT SHIFT:
*    ROUTINE TO SHIFT ACC 'CNT' STEPS TO THE RIGHT.
*    SIGN EXTENSION IS ASSUMED TO BE DONE ALLREADY
*
@RSH16   CLC  1(2,XR2),ZERO     CHECK IF SHIFT COUNT == 0
         BE   0(,ARR)           RETURN!
         CLC  1(2,XR2),N16      CHECK IF SHIFT  COUNT >= 16
         JNH  L1
         MVC  ACC(2),ACC_SGN    RESULT OF 16 RIGHT SHIFTS
         B    0(,ARR)           RETURN!
L1       MVC  CNT,1(,XR2)
*
LOOP     SRC  ACC(4),1          SHIFT ONE STEP RIGHT (32 BIT SHIFT)
         SLI  CNT,1
         JNZ  LOOP              MORE STEPS TO SHIFT
         B    0(,ARR)           RETURN!
*
*****
*
CNT      DS    XL1
ZERO     DC    XL2'0000'
N16      DC    IL2'16'
         END
