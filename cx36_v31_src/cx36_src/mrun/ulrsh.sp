*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @ULRSH:  LONG RIGHT SHIFT BY INTEGER COUNT                           *
*           FOR IBM SYSTEM/36                                           *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*
@ULRSH   START
         ENTRY @RSH32
         EXTRN @AC0
*
XR2      EQU   2
ARR      EQU   8
PC       EQU   X'10'
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_SGN  EQU   @AC0+3
*
*
         SLC   ACC_SGN(4),ACC_SGN       NO SIGN EXTENSION
*
************
*
*  LONG RIGHT SHIFT:
*    ROUTINE TO SHIFT ACC 'CNT' STEPS TO THE RIGHT.
*    SIGN EXTENSION IS ASSUMED TO BE DONE ALLREADY
*
@RSH32   CLC  1(2,XR2),ZERO     CHECK IF SHIFT COUNT == 0
         BE   0(,ARR)           RETURN!
         CLC  1(2,XR2),N32      CHECK IF SHIFT  COUNT >= 32
         JNH  L1
         MVC  ACC(4),ACC_SGN    RESULT OF 32 RIGHT SHIFTS
         B    0(,ARR)           RETURN!
L1       MVC  CNT,1(,XR2)
*
LOOP     SRC  ACC(8),1          SHIFT ONE STEP RIGHT (64 BIT SHIFT)
         SLI  CNT,1
         JNZ  LOOP              MORE STEPS TO SHIFT
         B    0(,ARR)           RETURN!
*
*****
*
CNT      DS    XL1
ZERO     DC    XL2'0000'
N32      DC    IL2'32'
         END
