*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @CENTS:   Enter C function and check stack for IBM SYSTEM/36         *
*                                                                       *
*       On entry XR1 holds offset to new frame pointer                  *
*       XR2 has address to return to in function                        *
*                                                                       *
*************************************************************************
*
@CENTS   START 0
*
#XR1     EQU   X'01'
#XR2     EQU   X'02'
#ARR     EQU   X'08'
*
FPOFFSET EQU   246
*
         EXTRN @FP
*
         A     @FP+1,#XR1                  calc. new frame pointer
* Check stack against stop here
         ST    FPOFFSET+3(,#XR1),#ARR      save ARR
         MVC   FPOFFSET+1(2,#XR1),@FP+1    save old frame pointer
         ST    @FP+1,#XR1                  update new frame pointer
         B     0(,#XR2)                    return to function
*
         END
