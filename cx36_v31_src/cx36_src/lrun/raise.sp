***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   raise an exception
*
***************************************************
_RAISE   START 0
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
ARGB     EQU   250
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
*   VOID _raise(TEXT **ptr, TEXT **cx)
*
         LA    L10,#XR2
         BD    @CENT                   enter routine
L10      EQU   *
         MVI   @AC0-8,0                reset "pop frame flag"
         CLC   ARGB+1(2,#XR1),@CONST-1 ptr == -1 ?
         JE    L12                     yes
         CLC   ARGB+1(2,#XR1),@CONST+3 ptr == 0 ?
         JNE   L13                     no
L11      EQU   *
         MVI   @AC0-8,1                set "pop frame flag"
L12      EQU   *
         MVC   ARGB+1(2,#XR1),ROOT+1   use root as start of handler chain
L13      EQU   *
         CLC   ARGB+1(2,#XR1),@CONST+3 new ptr == 0 ?
         JE    BOMB                    yes, bomb
         CLC   ARGB+3(2,#XR1),@CONST-1 cx == -1 ?
         JE    L15                     yes
         CLC   ARGB+3(2,#XR1),@CONST+3 cx == 0 ?
         JNE   L16                     no
L14      EQU   *
         MVI   @AC0-8,1                set "pop frame flag"
L15      EQU   *
         MVC   @AC0+7(2),@CONST+3      condno = 0
         J     LXX
L16      EQU   *
         L     ARGB+1(,#XR1),#XR2      get pointer to exception frame
         ST    @AC0-1,#XR2
         MVC   @AC0+7(2),@CONST+7      condno = 1
LWHILE   EQU   *
         CLC   11(2,#XR2),@CONST-1     cond == -1 ?
         JNE   L17                     no
         L     @AC0-1,#XR2             yes, get pointer to next
         L     1(,#XR2),#XR2              exception frame
         ST    @AC0-1,#XR2
         MVC   @AC0+7(2),@CONST+7      condno = 1
         J     LWHILE
L17      EQU   *
         CLC   11(2,#XR2),@CONST+3     cond == 0 ?
         JNE   L18                     no
         MVI   @AC0-8,1                yes, set "pop frame flag"
         J     LXX                     and break loop
L18      EQU   *
         CLC   11(2,#XR2),ARGB+3(,#XR1) cond == cx ?
         JE    LXX                     yes, condition found
         LA    2(,#XR2),#XR2           no, point to next cond on frame
         ALC   @AC0+7(2),@CONST+7      condno++
         J     LWHILE                  another round on the loop
LXX      EQU   *
         L     @AC0-1,#XR2             get pointer to frame
         CLI   @AC0-8,1                pop frame ?
         JNE   LXX2                    no
         CLC   9(2,#XR2),@CONST+3      exc_frame[8] (e_f.ptr) == 0 ?
         JE    LXX1                    yes
         L     9(,#XR2),#XR2           else
         MVC   1(2,#XR2),ARGB+3(,#XR1)   *e_f.ptr = cx
         L     @AC0-1,#XR2             get pointer to frame again
LXX1     EQU   *
         MVC   ROOT+1,1(,#XR2)         set root to "older" frame
         ST    @EFPTR+1,#XR2           free the frame space
LXX2     EQU   *
         L     7(,#XR2),#ARR           restore address recall register
         L     5(,#XR2),#XR1           restore frame pointer
         ST    @FP+1,#XR1
         B     0(,#ARR)                return to willing _when()
BOMB     EQU   *
         CLC   ARGB+3(2,#XR1),@CONST+3 cx == 0 ?
         JNE   BOMB1                   no
         MVC   243(2,#XR1),MSGPTR+1    point to default text
         J     BOMB2
BOMB1    EQU   *
         MVC   243(2,#XR1),ARGB+3(,#XR1) else msg is cx
BOMB2    EQU   *
         MVC   245(2,#XR1),@CONST+3    arg 2 is 0
         LA    -8,#XR1
         B     ERROR                   write error msg
         B     @CRET                   should never return
*
MSGPTR   EQU   *
         DC    AL2(MSGSTR)
MSGSTR   EQU   *
         DC    CL19'unchecked condition'
         DC    XL1'00'
         END
