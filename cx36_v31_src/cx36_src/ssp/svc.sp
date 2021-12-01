*  SUPERVISOR CALL UNDER IBM SYSTEM/36
*  copyright (c) 1986 by Whitesmiths, Ltd.
*  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
*
*  This is an function that can be called from any C program, that
*  makes it possible to do allmost every supervisor call available
*  under system/36.  There is one (known) exception; if a certain call
*  returnes valuable data in xr1, it is guaranteed to be destroyed.
*  
*  The C synopsis in Whitesmith's style will look as follows:
*
*  VOID _svc(ilc, xrs, Rbyte, Qbyte, ...)
*      UTINY ilc;    /* number of bytes inline */
*      UTINY xrs;    /* index register selector */
*      UTINY Rbyte;  /* R-byte in svc code */
*      UTINY Qbyte;  /* Q-byte in svc code */
*
*  The arguments that may follow the first four, is:
*
*       One to three character arguments, which corresponds to the inline
*       bytes normally inserted after the svc code in assembler.  The ilc
*       argument must contain the number of inlines bytes you wish to pass.
*
*       One integer argument which index register one will be loaded with
*       before the real svc call if the xrs argument equals one or three.
*
*       One integer argument which index register two will be loaded with
*       before the real svc call if the xrs argument equals two or three.
*
* DEFINITIONS
*
#XR1     EQU   X'01'               INDEX REGISTER 1
#XR2     EQU   X'02'               INDEX REGISTER 2
#ARR     EQU   X'08'               ADDRESS RECALL REGISTER
#IAR     EQU   X'10'               INSTRUCTION ADDRESS REGISTER
*
ARGBIAS  EQU   250                 OFFSET TO FIRST ARG FROM @FP+XR1
*
* MODULE AND C FUNCTION NAME
*
_SVC     START 0
*
* EXTERNAL SYMBOLS
*
         EXTRN @FP
*
*  ENTER FUNCTION CODE (MUST BE SUPPLIED IN C CALLABLE FUNCTIONS)
*
         A     @FP+1,#XR1          NEW FRAME POINTER VALUE IN XR1
         ST    249(,#XR1),#ARR     SAVE RETURN ADDRESS
         MVC   247(2,#XR1),@FP+1   SAVE OLD FRAME POINTER
         ST    @FP+1,#XR1          ASSIGN FRAME POINTER NEW VALUE
*
* SETUP XR2 TO POINT AT THE FIRST BYTE IN ARGUMENT 0
*
         LA    ARGBIAS(,#XR1),#XR2 XR2 -> ARGUMENT 0 
*
* SETUP XR1 TO POINT AT THE FIRST INLINE BYTE POSITION
*
         LA    DOSVC+3,#XR1        INLINE BYTE ADDRESS OF SVC CODE
*
* GET ALL FIXED ARGUMENTS (MUST BE FOUR)
*
         MVC   ILCNT(1),1(,#XR2)   INLINE BYTE COUNT FROM ARG0
         MVC   XRSEL(1),3(,#XR2)   INDEX REGISTER SELECTOR FROM ARG1
         MVC   DOSVC+2(1),5(,#XR2) R-BYTE (TYPE OF SVC) FROM ARG2
         MVC   DOSVC+1(1),7(,#XR2) Q-BYTE (FURTHER DEF) FROM ARG3
*
* MOVE EACH INLINE BYTE INTO THE SVC CALL CODE
*
LOOP     CLI   ILCNT,0             COMPARE INLINE COUNT WITH ZER0
         BE    PTCHJMP             IF EQUAL THEN BREAK
         MVC   0(1,#XR1),9(,#XR2)  MOVE LOW OF CURR ARG INTO SVC CODE
         LA    2(,#XR2),#XR2       ADD TWO TO XR2 -> NEXT ARG
         LA    1(,#XR1),#XR1       ADD ONE TO XR1 -> NEXT SVC BYTE
         SLI   ILCNT,1             SUBTRACT ONE FROM INLINE COUNT
         B     LOOP                CONTINUE
*
* COPY A BRANCH INSTRUCTION TO THE END OF THE SVC CODE
*
PTCHJMP  MVC   3(4,#XR1),JUMP+3    COPY THE BRANCH AT JUMP
*
* LOAD XR1 OR/AND XR2 FROM STACK IF NEEDED IN SVC CALL
*
         CLI   XRSEL,0             COMPARE XRSEL WITH ZERO
         BE    DOSVC               IF EQUAL THEN NO REGS IS NEEDED
         CLI   XRSEL,2             COMPARE XRSEL WITH TWO
         BE    LODXR2              IF EQUAL THEN ONLY XR2 IS NEEDED
LODXR1   L     9(,#XR2),#XR1       LOAD FROM CURRENT ARG TO XR1
         CLI   XRSEL,3             COMPARE XRSEL WITH THREE
         BNE   DOSVC               IF NOT EQUAL THEN SKIP XR2
         LA    2(,#XR2),#XR2       ADD TWO TO XR2 -> NEXT ARG
LODXR2   L     9(,#XR2),#XR2       LOAD FROM CURRENT ARG TO XR2
*
* PERFORM THE SUPERVISOR CALL
*
DOSVC    SVC   X'00',X'00'         SUPERVISOR CALL IS DONE HERE
         DC    XL3'00'             FILL AREA FOR INLINE AND BRANCH CODE
         B     FINISH              FILL AREA FOR BRANCH CODE
*
JUMP     B     FINISH              ONLY HERE TO BE COPIED
*
* FIXUP AND RETURN
*
FINISH   L     @FP+1,#XR1
         L     249(,#XR1),#ARR
         L     247(,#XR1),#XR1
         ST    @FP+1,#XR1
         B     0(,#ARR)
*
* VARIABLES
*
ILCNT    DC    XL1'00'             INLINE BYTE COUNT
XRSEL    DC    XL1'00'             INDEX REGISTER SELECTOR
*
         END
