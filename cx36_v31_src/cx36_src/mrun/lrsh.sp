*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*  @LRSH:   LONG RIGHT SHIFT BY INTEGER COUNT                           *
*           FOR IBM SYSTEM/36                                           *
*                                                                       *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*    IF LEFT IS NEGATIVE, SET SIGN EXTENSION TO -1, ELSE USE THE UNSIGNED
*    RIGHT SHIFT ROUTINE.
*
@LRSH    START
*
         EXTRN @ULRSH
         EXTRN @RSH32
         EXTRN @AC0
*
PC       EQU   X'10'
ACC      EQU   @AC0+7           (RIGHTMOST BYTE OF AC0)
ACC_HI   EQU   @AC0+4           (LEFTMOST BYTE OF AC0)
ACC_SGN  EQU   @AC0+3
*
         CLI   ACC_HI,X'80'             CHECK SIGN
         JL    POS
         MVC   ACC_SGN(4),MINUS         SET SIGN EXTENSION
         LA    @RSH32,PC                JUMP TO @RSH32
POS      LA    @ULRSH,PC                USE UNSIGNED SHIFT ROUTINE
*
*****
*
MINUS    DC    XL4'FFFFFFFF'
         END
