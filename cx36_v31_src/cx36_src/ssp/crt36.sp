 OPTIONS NOXREF
*
* Startup code for C program
*
@CRT36   START 0
*
         PRINT NODATA,NOGEN
*
         ENTRY @EXIT
*
         EXTRN @STTOP
         EXTRN _EXIT
         EXTRN _MAIN
         EXTRN _MEMRY
         EXTRN _STOP
         EXTRN _TERMI
*
         EXTRN _DATA            to link data.sp object module
         EXTRN @CONST
         EXTRN @AC0
         EXTRN @FP
         EXTRN _ERRNO           to link lstat.c object module
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
FPOFFSET EQU   246
*
* Get 256 of Local Data Area into buffer
*
         LA    INFPARM,#XR2
         SVC   X'04',X'01'                 sys transient request
         DC    XL1'0F'                     information retreival transient
*
* Set up stack and call main program
*
         CLC   _STOP+1(2),@CONST+7         _stop == 1 ?
         JNE   L1                          no
         LA    _MEMRY,#XR1                 else set _stop = &_memry
         ST    _STOP+1,#XR1
L1       EQU   *
         LA    @STTOP,#XR1                 set up frame pointer
         S     FPOFS,#XR1
         ST    @FP+1,#XR1
         MVC   FPOFFSET-1(2,#XR1),ARGPTR   get pointer to arguments
         LA    -6,#XR1
         B     _MAIN                       enter C function
         MVC   FPOFFSET-1(2,#XR1),@AC0+7
         LA    -6,#XR1
         B     _TERMI
*
* End job if program file with no code called
*
@EXIT    EQU *
         $EOJ
*
* data for arguments
*
ARGPTR   DC    AL2(LDABUF)                 pointer to args
INFPARM  EQU   *
         DC    XL1'00'                     function byte
         DC    XL1'89'                     request byte, get local data
         DC    AL2(LDABUF)                 buffer address
         DC    XL3'0'
         DC    AL2(256-1)                  length
         DC    AL2(1-1)                    offset
LDABUF   EQU   *
         DC    256CL1' '                   LDA buffer
DUMBUF   EQU   *                           end of LDA or
         DC    XL1'0'                      dummy buffer if LDA not used
*
* constants
*
FPOFS    DC    AL2(FPOFFSET)
*
         END
