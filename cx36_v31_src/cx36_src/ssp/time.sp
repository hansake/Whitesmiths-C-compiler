***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   get system time
*
***************************************************
_TIME    START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @CENT
         EXTRN @CRET
*
ARGBIAS  EQU   250
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
*
*   ULONG _time()
*
         LA    L20,#XR2
         BD    @CENT                   enter routine
L20      EQU   *
         LA    TRB,#XR2
         MVI   0(,#XR2),4              time in binary
         SVC   80,0
         DC    XL3'0A4000'
         MVC   @AC0+7(4),TRBTIME(,#XR2)   return time
         B     @CRET
*
*   timer request block offsets
*
TRBFLG1  EQU   0,1              flag byte one, function byte
TRBFLG2  EQU   TRBFLG1+1,1      flag byte two, timer id
TRBTIME  EQU   TRBFLG2+6,6      time field
TRBDATE  EQU   TRBTIME+6,6      date field
*
*   timer request block
*
TRB      DC    XL1'0'
         DC    XL1'0'
         DC    XL6'0'
         DC    XL6'0'
*
         END
