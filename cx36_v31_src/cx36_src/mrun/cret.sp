*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @CRET:   Return from C function for IBM SYSTEM/36                    *
*                                                                       *
*************************************************************************
*
@CRET    START 0
*
#XR1     EQU   X'01'
#ARR     EQU   X'08'
*
FPOFFSET EQU   246
*
         EXTRN @FP
*
         L FPOFFSET+3(,#XR1),#ARR    get return address
         L FPOFFSET+1(,#XR1),#XR1    restore XR1
         ST @FP+1,#XR1               and @FP
         B 0(,#ARR)                  then return
*
         END
