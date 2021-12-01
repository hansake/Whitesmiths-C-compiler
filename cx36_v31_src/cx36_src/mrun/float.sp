 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  Floating point registers for IBM System/36                           *
*                                                                       *
*************************************************************************
*
@FENTR   START 0
*
         PRINT NODATA,NOGEN
*
#XR1     EQU   X'01'
#XR2     EQU   X'02'
#ARR     EQU   X'08'
#IAR     EQU   X'10'
#WR4     EQU   X'44'
#WR5     EQU   X'45'
#WR6     EQU   X'46'
#WR7     EQU   X'47'
*
         EXTRN @AC0
         EXTRN @FP
* 
         ENTRY @FENTR
         ENTRY @FESAV
         ENTRY @FESIS
         ENTRY @FLEAV
         ENTRY @WR7SA
*
*   Entry saving regsters to sceintific instructions
*
         ST    ARRSAV+1,#ARR       save ARR, return address
         ST    @WR7SA+1,#WR7       save WR7, pointer to right operand
         ST    XR2SAV+1,#XR2       save XR2
*
*   Entry to sceintific instructions
*
@FESIS   EQU   *
         ST    WR6SAV+1,#WR6       save WR6, address of scientific inst.
         LA    $SCCOM,#XR1         set communications area
         LA    $SAVEA,#XR2         set save area
         L     WR6SAV+1,#ARR       ARR for scientific instruction
         XFER  1,1                 enter scientific mode at ARR
*
*   Entry saving registers
*
@FESAV   EQU   *
         ST    ARRSAV+1,#ARR       save ARR, return address
         ST    @WR7SA+1,#WR7       save WR7, pointer to right operand
         ST    WR6SAV+1,#WR6       save WR6, address of next inst.
         ST    XR2SAV+1,#XR2       save XR2
         L     @WR7SA+1,#XR2       get pointer to left arg in XR2
         L     WR6SAV+1,#ARR
         B     0(,#ARR)
*
*   Return
*
@FLEAV   EQU   *
         L     @FP+1,#XR1          restore XR1
         L     XR2SAV+1,#XR2       restore XR2
         L     ARRSAV+1,#ARR       restore ARR
         B     0(,#ARR)            return to caller
*
*   Register save areas
*
ARRSAV   EQU   *
         DC    XL2'0'
WR6SAV   EQU   *
         DC    XL2'0'
@WR7SA   EQU   *
         DC    XL2'0'
XR2SAV   EQU   *
         DC    XL2'0'
*
*  Sceintific communication area
*
         $CSET
*
         END
