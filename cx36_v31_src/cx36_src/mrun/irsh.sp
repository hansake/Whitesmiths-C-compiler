*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @IRSH:   INTEGER RIGHT SHIFT BY INTEGER COUNT                        *
*           FOR IBM SYSTEM/36                                           *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*    IF LEFT IS NEGATIVE, SET SIGN EXTENSION TO -1, ELSE USE UNSIGNED 
*    RIGHT SHIFT ROUTINE.
*
@IRSH    START
*
         EXTRN @UIRSH           FILE CONTAINING @RSH16
         EXTRN @RSH16
         EXTRN @AC0
*
PC       EQU   X'10'
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+6           (LEFTMOST BYTE OF AC0)
ACC_SGN  EQU   @AC0+5
*
         CLI   ACC_HI,X'80'         CHECK SIGN
         JL    POS
         MVC   ACC_SGN(2),MINUS     SET SIGN EXTENSION
         LA    @RSH16,PC            JUMP TO @RSH16
POS      LA    @UIRSH,PC            USE UNSIGNED RIGHT SHIFT
*
*****
*
MINUS    DC    XL2'FFFF'
         END
