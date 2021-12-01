 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986, 1987 BY UNISOFT AB, SWEDEN                       *
*                                                                       *
*  @STACK: defines stack + heap size for C program                      *
*          also defines exception stack size                            *
*          (the exception stack is used by routines raise and when)     *
*                                                                       *
*************************************************************************
*
@STACK   START 0
*
         PRINT NODATA,NOGEN
*
         ENTRY @STTOP         top of stack address
         ENTRY _MEMRY         start of heap address
*
         ENTRY @EFPTR         exception frame stack pointer
         ENTRY @EFEND         exception frame stack end
         ENTRY ROOT           root of exception handler chain
*
@EFPTR   EQU   *
         DC    AL2(EFRAME)
@EFEND   EQU   *
         DC    AL2(EFREND)
ROOT     EQU   *              root of exception handler chain
         DC    IL2'0'
***********************************************************************
EFRAME   EQU   *
         DS    48CL1         change this to change exception stack size
*                            now 48 bytes
*                            every exception takes at least 12 bytes
EFREND   EQU   *
***********************************************************************
         ORG   *,8,0
_MEMRY   EQU   *
         DS    4096CL1       change this to change heap+stack size
*                            now 4096 bytes
@STTOP   EQU   *
***********************************************************************
*
         END
