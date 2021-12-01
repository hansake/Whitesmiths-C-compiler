***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   setjmp and longjmp functions for stack
*   manipulations
*
***************************************************
_SETJM   START 0
*
         ENTRY _LONGJ
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
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
*   _setjmp(SAVEAREA *p)
*
         A     @FP+1,#XR1
         L     ARGBIAS+1(,#XR1),#XR2       get env pointer
         ST    1(,#XR2),#ARR               save ARR
         MVC   3(2,#XR2),@FP+1             save @FP
*
         LA    0,#WR7                      return 0
         ST    @AC0+7,#WR7
*
         L     @FP+1,#XR1
         B     0(,#ARR)
*
*   _longjmp(SAVEAREA *p, BYTES val)
*
_LONGJ   EQU   *
         A     @FP+1,#XR1
         L     ARGBIAS+1(,#XR1),#XR2       get env pointer
         CLC   ARGBIAS+3(,#XR1),@CONST+3   if (val == 0)
         JNE   NOTZERO
         ALC   ARGBIAS+3(2,#XR1),@CONST+7      val += 1
NOTZERO  EQU   *
         L     ARGBIAS+3(,#XR1),#WR7       get val
         L     1(,#XR2),#ARR               restore ARR
         L     3(,#XR2),#XR1               restore @FP
         ST    @FP+1,#XR1
         ST    @AC0+7,#WR7                 return val
         B     0(,#ARR)
*
         END
