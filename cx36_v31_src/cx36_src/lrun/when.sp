***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   handle exceptions
*
***************************************************
_WHEN    START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @EFPTR          exception frame stack pointer
         EXTRN @EFEND          exception frame stack end
         EXTRN @CENT
         EXTRN @CRET
         EXTRN ERROR
         EXTRN ROOT
*
ARGB     EQU   250             stack argument bias
FPOF     EQU   ARGB-4          stack frame pointer offset
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
*   COUNT _when(TEXT **ptr, TEXT **c1, TEXT **c2, ..., TEXT **cend)
*
         LA    L10,#XR2
         BD    @CENT                   enter routine
L10      EQU   *
         L     @EFPTR+1,#WR4           WR4 = pointer to new exception frame
         CLC   ARGB+1(2,#XR1),@CONST+3 ptr == 0 ?
         JE    L11                     yes
         L     ARGB+1(,#XR1),#XR2      no, store address of frame in *ptr
         ST    1(,#XR2),#WR4
L11      EQU   *
         L     @EFPTR+1,#XR2           XR2 = pointer to new exception frame
         ALC   @EFPTR+1(2),CONS10      add 10 to exception stack pointer
         CLC   @EFPTR+1(2),@EFEND+1    is there any room on stack ?
         JH    BOMB                    no space left
         MVC   1(2,#XR2),ROOT+1        exc_frame[0] = root
         MVC   3(2,#XR2),@CONST+19     exc_frame[2] = 4, not used
         MVC   5(2,#XR2),FPOF+1(,#XR1) exc_frame[4] = frame pointer
         MVC   7(2,#XR2),FPOF+3(,#XR1) exc_frame[6] = address recall reg.
         MVC   9(2,#XR2),ARGB+1(,#XR1) exc_frame[8] = ptr
LWHILE   EQU   *
         MVC   11(2,#XR2),ARGB+3(,#XR1) exc_frame[n] = cn
         ALC   @EFPTR+1(2),@CONST+11   add 2 to exception stack pointer
         CLC   ARGB+3(2,#XR1),@CONST+3 cn == 0 ?
         JE    LXX                     yes, exit loop
         CLC   ARGB+3(2,#XR1),@CONST-1 cn == -1 ?
         JE    LXX                     yes, exit loop
         CLC   @EFPTR+1(2),@EFEND+1    is there any room on stack ?
         JH    BOMB                    no space left
         LA    2(,#XR1),#XR1           next argument on stack
         LA    2(,#XR2),#XR2           next element on exc_frame
         J     LWHILE
LXX      EQU   *
         ST    ROOT+1,#WR4             set root pointing to last frame
         MVC   @AC0+7(2),@CONST-1      return -1
         L     @FP+1,#XR1
         B     @CRET
BOMB     EQU   *
         L     @FP+1,#XR1
         MVC   243(2,#XR1),MSGPTR+1    point to error text
         MVC   245(2,#XR1),@CONST+3    arg 2 is 0
         LA    -8,#XR1
         B     ERROR                   write error msg
         B     @CRET                   should never return
*
CONS10   DC    IL2'10'                 constant 10
MSGPTR   EQU   *
         DC    AL2(MSGSTR)             pointer to error msg. if no space
MSGSTR   EQU   *
         DC    CL25'exception stack too small'
         DC    XL1'00'
         END
