*************************************************************************
*  COPYRIGHT (C) 1986 BY UNISOFT AB, SWEDEN                             *
*                                                                       *
*  @JTAB:   PERFORM C INTEGER SWITCH STATEMENT FOR IBM SYSTEM/36        *
*                                                                       *
*       case value in AC0                                               *
*       XR2 points to jump table                                        *
*                                                                       *
*************************************************************************
*
@JTAB    START 0
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
         EXTRN @AC0
         EXTRN @FP
         EXTRN @XR2
         EXTRN @CONST
*
T0       EQU *
         CLC 1(2,#XR2),@CONST+3      test if end of table (== 0)
         JNE T1
         L 3(,#XR2),#ARR             jump to default entry
         B 0(,#ARR)
T1       EQU *
         CLC 3(2,#XR2),@AC0+7        test if value is right
         JNE T2
         L 1(,#XR2),#ARR             jump to case entry
         B 0(,#ARR)
T2       EQU *
         LA 4(,#XR2),#XR2            get next entry
         J T0 
*
         END
