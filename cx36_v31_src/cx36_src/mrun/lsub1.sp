*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*  WRITTEN 1986 BY TOMAS OLOVSSON FOR UNISOFT AB, SWEDEN                *
*                                                                       *
*                                                                       *
*  @LSUB1:  IBM SYSTEM/36  SUBROUTINES FOR LOGICAL INSTRUCTIONS         *
*                                                                       *
*************************************************************************
*
@LSUB1   START
         ENTRY @LSUB2
         ENTRY @LONG
         ENTRY @A1
         ENTRY @A2
*
         EXTRN @AC0
         EXTRN @FP              SAVE AREA FOR XR1
*
XR1      EQU   1
XR2      EQU   2
ARR      EQU   8
PC       EQU   X'10'
*
ACC      EQU   @AC0+7           RIGHTMOST BYTE, TYPE == LONG
ACCHI    EQU   @AC0+5           HIGH PART OF ACC, TYPE == INT
ACCX     EQU   @AC0+9           2 BYTES AFTER ACC
ARG2     EQU   1                OFFSET FOR RIGHTMOST BYTE, TYPE == INT
*
*
***********************************************************************
*
*  LSUB1:       
*    A1 = (ACC & 0X5555) + (XR2 & 0X5555);
*    A2 = (ACC & 0XAAAA) + (XR2 & 0XAAAA);   (MAY GIVE OVERFLOW!)
*
*
         MVC   @A1,ACC          A1 = ACC & 5555
         SBF   @A1,X'AA'
         SBF   @A1-1,X'AA'
         MVC   TMP,ARG2(,XR2)   TMP = XR2 & 5555
         SBF   TMP,X'AA'
         SBF   TMP-1,X'AA'
         ALC   @A1,TMP          A1 = A1 + TMP
*
         MVC   @A2,ACC          A2 = ACC & AAAA
         SBF   @A2,X'55'
         SBF   @A2-1,X'55'
         MVC   TMP,ARG2(,XR2)   TMP = XR2 & AAAA
         SBF   TMP,X'55'
         SBF   TMP-1,X'55'
         MVI   @A2-2,0          CLEAR UPPER HALF OF @A2
         ALC   @A2(3),TMP       A2 = A2 + TMP  (OVERFLOW IN A2U!)
*
         B     0(,ARR)          RETURN
*
*
************************************************************************
*
*  LSUB2:
*    ACC += (A1 & 0X5555) + (A2 & 0XAAAA);
*
*
@LSUB2   MVC   TMP,@A1
         SBF   TMP,X'AA'
         SBF   TMP-1,X'AA'
         ALC   ACC(2),TMP
         MVC   TMP,@A2
         SBF   TMP,X'55'
         SBF   TMP-1,X'55'
         ALC   ACC(2),TMP
*
         B     0(,ARR)          RETURN
*
*
************************************************************************
*
*  @LONG:       FUNCTION TO PERFORM A 32-BIT LOGICAL OPERATION.
*               IT IS DONE BY APPLYING THE 16-BIT FUNCTION TO EACH
*               ONE OF THE TWO HALVES OF THE OPERANDS:
*                  ACC = (ACC_LO "OP" ARG2_LO) + (ACC_HI "OP" ARG2_HI)
*
*               PARAMETERS FOR THE FUNCTION IN ACC AND 0(,XR2)
*               POINTER TO THE FUNCTION IN 'XR1' (OLD CONTENTS
*               SAVED IN @FP).
*               NOTE:  2 BYTES TO THE RIGHT OF ACC IS DESTROYED!
*
@LONG    ST    RETADDR,ARR      SAVE RETURN ADDRESS
*
         MVC   ACCX(4),ACC      MAKE ACC CONTAIN HIGH ORDER PART
         B     0(,XR1)          CALL 16-BIT FUNCTION
         MVC   ACCHI(2),ACC
         MVC   ACC(2),ACCX
*
         A     TWO,XR2          MAKE XR2 POINT TO LOW ORDER PART OF ARG2
         B     0(,XR1)          16-BIT OPERATION (RESULT NOW IN ACC!)
         S     TWO,XR2          RESTORE XR2
         L     @FP+1,XR1        RESTORE XR1
         L     RETADDR,PC       RETURN
*
*
************************************************************************
*
*
*
         DC    XL2'0'          UPPER HALF (MSB) OF A1
@A1      DS    XL2
*
         DC    XL2'0'          UPPER HALF (MSB) OF A2
@A2      DS    XL2
*
         DC    XL2'0'          UPPER HALF OF TMP
TMP      DS    XL2
*
TWO      DC    XL2'2'
RETADDR  DS    XL2
         END
