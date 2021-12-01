*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*                                                                       *
*  @LAND:  AND  LONG BY LONG FOR IBM SYSTEM/36                          *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*  METHOD:      IAND IS APPLIED TO BOTH HALVES OF THE OPERANDS:
*                   AC0 = (AC0_LO & ARG2_LO) + (AC0_HI & ARG2_HI)
*               THIS IS DONE BY CALLING @LONG WITH 'XR1' POINTING
*               TO THE FUNCTION @IAND.
*
*               NOTE:  2 BYTES TO THE RIGHT OF AC0 IS DESTROYED!
*
@LAND    START
         EXTRN @IAND
         EXTRN @LSUB1           FILE CONTAINING @LONG
         EXTRN @LONG
*
XR1      EQU   1
PC       EQU   X'10'
*
         LA    @IAND,XR1        XR1 NOW POINTS TO @IAND (OLD VALUE IN @FP)
         LA    @LONG,PC         PERFORM THE 32-BIT OPERATION!  (NO RETURN)
         END
