 OPTIONS NOXREF
*
* Common data and constant section for C program
*
_DATA    START 0
*
         PRINT NODATA,NOGEN
*
         ENTRY @AC0
         ENTRY @FP
         ENTRY @XR2
         ENTRY @CONST
*
* copyright
*
*_DATA    EQU   *
         DC    XL1'0'
         DC    CL52'C36 Version 3.11 (c) Copyright 1987 Whitesmiths Ltd.'
         DC    XL1'0'
*
* Constant table
*
         DC    IL4'-4'
         DC    IL4'-3'
         DC    IL4'-2'
         DC    IL4'-1'
@CONST   EQU   *
         DC    IL4'0'
         DC    IL4'1'
         DC    IL4'2'
         DC    IL4'3'
         DC    IL4'4'
*
* C runtime registers
*
         DC    XL8'0'
@AC0     EQU   *
         DC    XL8'0'
         DC    XL4'0'
@FP      EQU   *
         DC    AL2(X'0000')
@XR2     EQU   *
         DC    XL2'0'
*
         END
