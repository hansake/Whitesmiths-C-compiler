*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*                                                                       *
*  @IXOR:  XOR  INT BY INT FOR IBM SYSTEM/36                            *
*           LEFT OPERAND  IN AC0                                        *
*           XR2 POINTS TO FIRST BYTE OF RIGHT                           *
*           RETURN VALUE  IN AC0                                        *
*                                                                       *
*************************************************************************
*
*  METHOD:      
*    A1 = (AC0 & 0X5555) + (XR2 & 0X5555);      <-- SUB1
*    A2 = (AC0 & 0XAAAA) + (XR2 & 0XAAAA);      <-- SUB1
*    AC0 = 0;
*    AC0 += (A1 & 0X55555) + (A2 & 0XAAAA);     <-- SUB2
*
*
@IXOR    START
         EXTRN @LSUB1
         EXTRN @LSUB2
         EXTRN @AC0
*
ARR      EQU   8
PC       EQU   X'10'
ACC      EQU   @AC0+7          (RIGHTMOST BYTE OF AC0)
*
*
         ST    RETADDR,8       SAVE RETURN ADDRESS
         B     @LSUB1          SETS A1 AND A2
         SLC   ACC(2),ACC      ACC = 0
         L     RETADDR,ARR     SETUP RETURN ADDRESS FOR LSUB2
         LA    @LSUB2,PC       CALL @LSUB2  (NO RETURN!)
*
*
RETADDR  DS    CL2
         END
