***************************************************
*
*   copyright (c) 1986 by UNISOFT AB
*
*   extract integer from fraction part
*
***************************************************
_FRAC    START 0
*
         EXTRN @AC0
         EXTRN @FP
         EXTRN @CONST
         EXTRN @CENT
         EXTRN @CRET
         EXTRN @DADD
         EXTRN @DMUL
*
ARGBIAS  EQU   250
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
*
*   COUNT _frac(DOUBLE *pd, DOUBLE mul)
*
         LA    L20,#XR2
         BD    @CENT                   enter routine
L20      EQU   *
         LA    ARGBIAS(,#XR1),#XR2
** multiply
         LA    2(,#XR2),#WR7           &mul
         L     1(,#XR2),#XR2           pd
         MVC   @AC0+7(8),7(,#XR2)
         B     @DMUL                   *pd * mul
         MVC   7(8,#XR2),@AC0+7        move back product to *pd
         MVC   @AC0-1(2),@CONST+3      set return value to 0
         CLI   1(,#XR2),0              product == 0 ?
         JE    L30                     yes, return 0
** adjust exponent
L21      EQU   *
         MVC   @AC0-7(1),0(,#XR2)      get exponent
         SBF   @AC0-7,128              remove sign bit of exponent
         SLI   @AC0-7,64               and unbias the exponent
         JNH   L26                     if not 0 < exponent, no int part
         MVI   @AC0-8,0
         ALC   @AC0-7(2),@AC0-7        exponent <<= 2
         ALC   @AC0-7(2),@AC0-7
** adjust mantissa and exponent
L22      EQU   *
         TBF   1(,#XR2),128            msb of mantissa != 0 ?
         JF    L23                     yes
         ALC   7(7,#XR2),7(,#XR2)      not yet, shift mantissa left
         SLC   @AC0-7(2),@CONST+7      and substract one from exponent
         J     L22
L23      EQU   *
         TBF   @AC0-8,128              msb of exponent != 0 (negative) ?
         JF    L29                     yes, return 0
         CLC   @AC0-7(2),@CONST+3      exponent == 0 ?
         JE    L26                     yes
** extract frac and int
L24      EQU   *
         ALC   @AC0-1(2),@AC0-1        return value << 1
         TBF   1(,#XR2),128            msb of mantissa == 0 ?
         JT    L25                     yes
         ALI   @AC0-1,1                else, set lsb of return value
L25      EQU   *
         ALC   7(7,#XR2),7(,#XR2)      shift mantissa left
         SLC   @AC0-7(2),@CONST+7      decrement exponent
         JNZ   L24                     loop if exponent != 0
** restore exponent
L26      EQU   *
L27      EQU   *
         SBF   0(,#XR2),127            preserve sign bit of exponent
         ALI   0(,#XR2),64             and add exponent bias
** set sign of return value
         TBF   0(,#XR2),128            negative sign of mantissa ?
         JT    L28                     no
         LA    0,#WR4                  else, change sign
         S     @AC0-1,#WR4
         ST    @AC0-1,#WR4
         LA    -1,#WR5
         LA    DCNST1,#XR1
         J     L28A
L28      EQU   *
         LA    1,#WR5
         LA    DCNSTM1,#XR1
L28A     EQU   *
** adjust mantissa to interval [-0.5, 0.5]
         CLC   7(7,#XR2),DCNST05+7     -0.5 <= mantissa <= 0.5 ?
         JNH   L29                     yes
         MVC   @AC0+7(8),7(,#XR1)      constant -1.0 or 1.0
         LA    0(,#XR2),#WR7
         B     @DADD
         MVC   7(,#XR2),@AC0+7(8)
         A     @AC0-1,#WR5             adjust return value
         ST    @AC0-1,#WR5
** return
L29      EQU   *
         CLI   1(,#XR2),0              mantissa of *pd == 0 ?
         JNE   L31                     no
L30      EQU   *
         MVC   7(8,#XR2),DCNST0+7      set *pd = 0
L31      EQU   *
         MVC   @AC0+7(2),@AC0-1
         L     @FP+1,#XR1
         B     @CRET
*
DCNSTM1  EQU   *
         DC    XL8'C110000000000000'    -1.0
DCNST0   EQU   *
         DC    XL8'0000000000000000'     0.0
DCNST05  EQU   *
         DC    XL8'4080000000000000'     0.5
DCNST1   EQU   *
         DC    XL8'4110000000000000'     1.0
*
         END
