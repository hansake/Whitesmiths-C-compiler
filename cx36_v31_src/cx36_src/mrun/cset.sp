         MACRO
&SYSLAB  $CSET
         TEXT
         AGO   .COPYR                     SKIP COPYRIGHT GENERATION
* COPYRIGHT = 5727-AS1 (C) COPYRIGHT IBM CORP 1983
*             LICENSED MATERIAL - PROGRAM PROPERTY OF IBM
*             REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083.
.COPYR   ANOP
FOF0     EQU   *
         ENTRY #MNTRY
         ENTRY #SNTRY
         ENTRY #RNTRY
         ENTRY $SCCOM
         ENTRY #RETRN
         ENTRY DLDIRG
         ENTRY RESUME
         EXTRN @FOBB(131)
         EXTRN #UNITB(131)
         EXTRN @FOD0(131)
         EXTRN @FOI0(131)
         EXTRN @FOB1(131)
         EXTRN @FOB1X(131)
         EXTRN @FOD1(131)
*                      BEGIN SCIENTIFIC COMMUNICATIONS AREA
$FXEQ    EQU   0                  EXECUTION SWITCHES
$FXEQ1   EQU   $FXEQ+1            EXECUTION SWITCHES
$FTEST   EQU   $FXEQ1+1           FCTST BYTE
$FOVDV   EQU   $FTEST+1           FLOATING POINT RANGE SWITCH
$AL2CSA  EQU   $FOVDV+2           SAVE AREA ADDRESS REGISTER
$FINMT   EQU   $AL2CSA+1          MAINTAINENCE AREA
$SHRSW   EQU   $FINMT+4           SHARED BUFFER SWITCH
$SEQAP   EQU   $SHRSW+2           ADDRESS OF SEQ APPENDAGE
$#TERR   EQU   $SEQAP+2           ADDRESS OF ERROR ROUTINE
$ENDEQ   EQU   $#TERR+2           ADDRESS FOR END= BRANCH
$ERREQ   EQU   $ENDEQ+2           ADDRESS FOR ERR= BRANCH
$#IOENT  EQU   $ERREQ+2           SAVE AREA FOR ACTIVE DTF@
$UNITAB  EQU   $#IOENT+2          ADDRESS OF UNITS TABLE
$FINEDT  EQU   $UNITAB+1          I/O EDIT SAVE AREA
$FINIOC  EQU   $FINEDT+15
$IOCBO   EQU   $FINIOC            ACTIVE IOCB SAVE AREA
$MORIO   EQU   $IOCBO+2           ADDRESS FOR I/O REQUESTS
$BUFAD   EQU   $MORIO+2           ACTIVE BUFFER ADDRESS
$BUFLNG  EQU   $BUFAD+2           ACTIVE BUFFER LENGTH
$BUFLIM  EQU   $BUFLNG+2          ACTIVE BUFFER LENGTH
$BUFPTR  EQU   $BUFLIM+2          POSITION IN ACTIVE BUFFER
$DEDSW1  EQU   $BUFPTR+2          DED CONTROL SWITCHES
$FINCNS  EQU   $DEDSW1+1          BEGINNING OF CONSTANTS
$C001    EQU   $FINCNS+1          TWO-BYTE BINARY ONE
$C4LN1   EQU   $C001+4            FOUR-BYTE MINUS ONE
$C002    EQU   $C4LN1+2           TWO-BYTE MINUS TWO
$C40     EQU   $C002+1            ONE-BYTE MINUS 64
$C5L80   EQU   $C40+5             FIVE-BYTE CONSTANT VALUE
$ZEROS   EQU   $C5L80+2           TWO-BYTE CONSTANT ZERO
$FINTM1  EQU   $ZEROS+1           BEGINNING OF TEMPORARY AREA
$ERNUM   EQU   $FINTM1+21         EXECUTION ERROR INDICATOR
$XR      EQU   $ERNUM+2           INDEX REGISTER
$AR      EQU   $XR+2              ADDRESS REGISTER
$B0      EQU   $AR+1              BINARY REGISTER WORK AREA
$B1      EQU   $B0+1              *
$B2      EQU   $B1+1              *
$B3      EQU   $B2+1              *
$BREG    EQU   $B3                BINARY REGISTER
$B4      EQU   $B3+1              *
$B5      EQU   $B4+1              *
$B6      EQU   $B5+1              *
$B7      EQU   $B6+1              *
$B8      EQU   $B7+1              *
$B9      EQU   $B8+1              *
$B10     EQU   $B9+1              *
$FSIGN   EQU   $B10+1
$F0      EQU   $FSIGN+1           FLOATING POINT REGISTER WORK AREA
$F1      EQU   $F0+1              *
$F2      EQU   $F1+1              *
$F3      EQU   $F2+1              *
$RREG    EQU   $F3                SINGLE PRECISION FLOATING POINT REG
$F4      EQU   $F3+1              *
$F5      EQU   $F4+1              *
$F6      EQU   $F5+1              *
$F7      EQU   $F6+1              *
$FREG    EQU   $F7                DOUBLE PRECISION FLOATING POINT REG
$F8      EQU   $F7+1              *
$F9      EQU   $F8+1              *
$F10     EQU   $F9+1              *
$F11     EQU   $F10+1             *
$F12     EQU   $F11+1             *
$F13     EQU   $F12+1             *
$F14     EQU   $F13+1             *
$F15     EQU   $F14+1             *
$F16     EQU   $F15+1             *
$F17     EQU   $F16+1             *
$F18     EQU   $F17+1             *
$F19     EQU   $F18+1             *
$F20     EQU   $F19+1             *
$F21     EQU   $F20+1             *
$XM      EQU   $F21+2             INDEX MULTIPLIER REGISTER
$IAREG   EQU   $XM+2              INSTRUCTION ADDRESS REGISTER
$OPREG   EQU   $IAREG+1           OPCODE REGISTER
$CCREG   EQU   $OPREG+1           CONDITION CODE REGISTER
$DMEMO   EQU   $CCREG+8           TEMP FOR REAL*8 EXTENSION
&SYSLAB  B     $MN&SYSNDX         BRANCH AROUND TABLES
         SPACE 3
$SCCOM   EQU   *                  BEGIN SCIENTIFIC COMMUNICATIONS AREA
FXEQ     DC    XL1'00'            $FXEQ    EXECUTION SWITCHES
FXEQ1    DC    XL1'00'            $FXEQ1   EXECUTION SWITCHES
FTEST    DC    XL1'00'            $FTEST   FCTST BYTE
FOVDV    DC    XL1'02'            $FOVDV   FLOATING POINT RANGE SWITCH
AL2CSA   DS    CL2                $AL2CSA  SAVE AREA ADDRESS REGISTER
FINMNT   EQU   *
         DC    CL4'VECT'          MAINTANENCE AREA
SHRSW    DC    XL1'00'            $SHRSW   SHARED BUFFER SWITCH
SEQAP    DC    XL2'0000'          $SEQAP   ADDRESS OF SEQ APPENDAGE
#TERR    DC    AL2(@FOBB)         $#TERR   ADDRESS OF ERROR ROUTINE
ENDEQ    DC    XL2'0000'          $ENDEQ   ADDRESS FOR END= BRANCH
ERREQ    DC    XL2'0000'          $ERREQ   ADDRESS FOR ERR= BRANCH
#IOENT   DC    XL2'0000'          $#IOENT  SAVE AREA FOR ACTIVE DTF@
UNITAB   DC    AL2(#UNITB)        $UNITAB  ADDRESS OF UNITS TABLE
FINEDT   EQU   *
         DS    CL15               I/O EDIT SAVE AREA
FINIOC   EQU   *
IOCB0    DC    IL1'00'            $IOCB0   ACTIVE IOCB SAVE AREA
MOREIO   DC    IL2'00'            $MOREIO  ADDRESS FOR I/O REQUESTS
BUFAD    DC    IL2'00'            $BUFAD   ACTIVE BUFFER ADDRESS
BUFLNG   DC    IL2'00'            $BUFLNG  ACTUVE BUFFER LENGTH
BUFLIM   DC    IL2'00'            $BUFLIM  END OF ACTIVE BUFFER
BUFPTR   DC    IL2'00'            $BUFPTR  POSITION IN ACTIVE BUFFER
DEDSW1   DC    IL2'00'            $DEDSW1  DED CONTROL SWITCHES
FINCNS   EQU   *                    BEGINNING OF CONSTANTS
C001     DC    IL2'1'             $C001    TWO-BYTE BINARY ONE
C4LN1    DC    XL4'FFFFFFFF'      $C4LN1   FOUR-BYTE MINUS ONE
C002     DC    XL2'0002'          $C002    TWO-BYTE BINARY TWO
C40      DC    XL1'40'            $C40     ONE-BYTE CONSTANT 64
C5L80    DC    XL5'8000000000'    $C5L80   FIVE-BYTE CONSTANT VALUE
ZEROES   DC    XL2'0000'          $ZEROS   TWO-BYTE BINARY ZERO
FINTM1   EQU   *                    BEGINNING OF TEMPORARY AREA
         DS    CL21               TEMPORARY AREA
ERNUM    DC    XL1'00'            $ERNUM   EXECUTION ERROR INDICATOR
         SPACE 1
XR       DC    XL2'00'            $XR      INDEX REGISTER
AR       DC    XL2'00'            $AR      ADDRESS REGISTER
B0       DC    XL1'00'            $B0 BINARY REGISTER WORK AREA
B1       DC    XL1'00'            $B1  *
B2       DC    XL1'00'            $B2    *
B3       DC    XL1'00'            $B3      *
         ORG   *-4
BREG     DS    XL4                $BREG    BINARY REGISTER
B4       DC    XL1'00'            $B4        *
B5       DC    XL1'00'            $B5          *
B6       DC    XL1'00'            $B6            *
B7       DC    XL1'00'            $B7              *
B8       DC    XL1'00'            $B8                *
B9       DC    XL1'00'            $B9                  *
B10      DC    XL1'00'            $B10                   *
FSIGN    DC    XL1'00'            $FSIGN
F0       DC    XL1'00'            $F0 FLOATING POINT REGISTER WORK AREA
F1       DC    XL1'00'            $F1  *
F2       DC    XL1'00'            $F2   *
F3       DC    XL1'00'            $F3    *
         ORG   *-4
RREG     DS    XL4                $RREG    SINGLE PRECISION REGISTER
F4       DC    XL1'00'            $F4     *
F5       DC    XL1'00'            $F5      *
F6       DC    XL1'00'            $F6       *
F7       DC    XL1'00'            $F7        *
         ORG   *-8
FREG     DS    XL8                $FREG    DOUBLE PRECISION REGISTER
F8       DC    XL1'00'            $F8         *
F9       DC    XL1'00'            $F9          *
F10      DC    XL1'00'            $F10          *
F11      DC    XL1'00'            $F11           *
F12      DC    XL1'00'            $F12            *
F13      DC    XL1'00'            $F13             *
F14      DC    XL1'00'            $F14              *
F15      DC    XL1'00'            $F15               *
F16      DC    XL1'00'            $F16                *
F17      DC    XL1'00'            $F17                 *
F18      DC    XL1'00'            $F18                  *
F19      DC    XL1'00'            $F19                   *
F20      DC    XL1'00'            $F20                    *
F21      DC    XL1'00'            $F21                     *
XM       DC    XL2'00'            $XM      INDEX MULTIPLIER REGISTER
IAREG    DC    XL2'00'            $IAREG   INSTRUCTION ADDRESS REGISTER
OPREG    DC    XL1'00'            $OPREG   OPCODE REGISTER
CCREG    DC    XL1'00'            $CCREG   CONDITION CODE REGISTER
DMEMO    DC    XL8'00'            $DMEMO   TEMP FOR REAL*8 EXTENSION
         DC    AL2(@FOI0)         ADDRESS OF IO CONTROL MODULE
         DC    AL2(@FOB1)         ADDRESS OF DED MODULE
         DC    AL2(@FOD1)         ADDRESS OF DODED MODULE
         DC    AL2(@FOB1X)        ADDRESS OF DEDX MODULE
GO       DC    AL2(GOTO)          ADDRESS OF GOTO XFER INSTRUCTION
FDATSW   DC    XL3'00'            PSEUDO DATA SWITCHES
FCMDKY   DC    XL3'00'            COMMAND KEY SETTING
         SPACE 3
SAVARR   DC    XL2'00'            SAVE AREA FOR CALLER'S ARR
MSGPL    EQU   *
MSGOPT   DC    XL1'CA'
         DC    XL1'00'
MIC      DC    XL2'0090'
*        #EQU
XR1      EQU   1                  REGISTER ONE
XR2      EQU   2                  REGISTER TWO
PSR      EQU   4                  PROGRAM STATUS REGISTER
ARR      EQU   8                  ADDRESS RECALL REGISTER
IAR      EQU   16                 INSTRUCTION ADDRESS REGISTER
IOB      EQU   1                  IOB POINTER
DTF      EQU   2                  DTF POINTER
@        EQU   3                  ADDRESS LENGTH
#        EQU   0                  ALTERABLE CODE
##       EQU   0                  ALTERABLE CODE
BLANK    EQU   C' '               BLANK CHARACTER
ZERO     EQU   0                  **
ONE      EQU   1                  * *
TWO      EQU   2                  *  *
THREE    EQU   3                  *   *
FOUR     EQU   4                  *    *
FIVE     EQU   5                  *     *  SELF DEFINING TERMS
SIX      EQU   6                  *    *
SEVEN    EQU   7                  *   *
EIGHT    EQU   8                  *  *
NINE     EQU   9                  **
*
*  ENVIRONMENT ROUTINES
*
#MNTRY   EQU   *
*
*        ENTRY TO E-MACHINE
*
         LA    $SCCOM,XR1         GET FINCOM ADDRESS IN XR1
*         XFER  MAIN
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'01'            R-BYTE
#SNTRY   EQU   *
*         XFER SUBR
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'02'            R-BYTE
#RNTRY   EQU   *
*         XFER  RNTRY
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'03'            R-BYTE
#RETRN   EQU   *
*         XFER  RETURN
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'04'            R-BYTE
RESUME   EQU   *
DLDIRG   EQU   *
*         XFER  NSI
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'01'            R-BYTE
GOTO     EQU   *
*         XFER  GO
         DC    XL1'F5'            XFER INSTRUCTION
         DC    IL1'01'            Q-BYTE
         DC    IL1'07'            R-BYTE
         SPACE 3
* SCIENTIFIC MAIN SAVE AREA
$SAVEA   DC    XL2'0080'          MAIN INDICATOR
         AIF   (T'&SYSLAB EQ 'O').DNAME
         DC    CL6'&SYSLAB'       MAIN PROGRAM NAME
         AGO   .NCONT
.DNAME   ANOP
         DC    CL6'MAIN'               MAIN PROGRAM NAME
.NCONT   ANOP
         SPACE 3
$MN&SYSNDX EQU *                  TAG FOR BRANCH INSTRUCTION
         MEND
