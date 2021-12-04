 OPTIONS NOXREF
 HEADERS CATG-0
* C36 ASSEMBLER CODE, V3.12
MAIN START 0
 BD $TEXT
*    1  #define _CVERSION       300
*    2  #define _LVERSION       300
*    3  #define _FPPHARD
*    4  #define _EBCDIC
*    5  #define _OS     ssplim.h
*    6  #define _MACH   m36.h
*    1  #include <wslxa.h>
*    2  #include <stdio.h>
*    3  #include <fcntl.h>
*    4  
*    5  FD fd = {0};
$DATA EQU *
 DS 2CL1
*    6  FILE *fp = {0};
 ENTRY FP
FP EQU *
 DS 2CL1
$5 EQU *
 DC XL5'9799A37A00'
$51 EQU *
 DC XL16'888593939640A6969993847A40F12500'
$52 EQU *
 DC XL2'A600'
$53 EQU *
 DC XL16'888593939640A6969993847A40F22500'
$54 EQU *
 DC XL16'888593939640A6969993847A40F32500'
$55 EQU *
 DC XL16'888593939640A6969993847A40F42500'
$56 EQU *
 DC XL16'888593939640A6969993847A406C8925'
 DC XL1'00'
$57 EQU *
 DC XL16'888593939640A6969993847A40F62500'
$501 EQU *
 DC XL16'888593939640A6969993847A406C8925'
 DC XL1'00'
*    7  
*    8  BOOL main()
*    9          {
$TEXT EQU *
 LA $62,#XR2
 BD @CENT
$62 EQU *
*   10  
*   11          fd = _open("prt:", O_WRONLY, 0, NULL );
 MVC 245(2,#XR1),@CONST+3
 MVI 242(,#XR1),0
 MVI 243(,#XR1),0
 MVI 240(,#XR1),0
 MVI 241(,#XR1),1
 MVC 239(2,#XR1),$4+1
 LA -12,#XR1
 B _OPEN
 MVC FD+1(2),@AC0+7
*   12          _write(fd, "hello world: 1\n", 15 );
 MVI 244(,#XR1),0
 MVI 245(,#XR1),15
 MVC 243(2,#XR1),$6+1
 MVC 241(2,#XR1),FD+1
 LA -10,#XR1
 B _WRITE
*   13          fp = fdopen(fd, "w");
 MVC 245(2,#XR1),$01+1
 MVC 243(2,#XR1),FD+1
 LA -8,#XR1
 B FDOPEN
 MVC FP+1(2),@AC0+7
*   14          fwrite("hello world: 2\n", 1, 15, fp);
 MVC 245(2,#XR1),FP+1
 MVI 242(,#XR1),0
 MVI 243(,#XR1),15
 MVI 240(,#XR1),0
 MVI 241(,#XR1),1
 MVC 239(2,#XR1),$21+1
 LA -12,#XR1
 B FWRITE
*   15          fputs("hello world: 3\n", fp);
 MVC 245(2,#XR1),FP+1
 MVC 243(2,#XR1),$41+1
 LA -8,#XR1
 B FPUTS
*   16          fprintf(fp, "hello world: 4\n");
 MVC 245(2,#XR1),$61+1
 MVC 243(2,#XR1),FP+1
 LA -8,#XR1
 B FPRINT
*   17          fprintf(fp, "hello world: %i\n", 5);
 MVI 244(,#XR1),0
 MVI 245(,#XR1),5
 MVC 243(2,#XR1),$02+1
 MVC 241(2,#XR1),FP+1
 LA -10,#XR1
 B FPRINT
*   18          putf(fp, "hello world: 6\n");
 MVC 245(2,#XR1),$22+1
 MVC 243(2,#XR1),FP+1
 LA -8,#XR1
 B PUTF
*   19          putf(fp, "hello world: %i\n", 7);
 MVI 244(,#XR1),0
 MVI 245(,#XR1),7
 MVC 243(2,#XR1),$42+1
 MVC 241(2,#XR1),FP+1
 LA -10,#XR1
 B PUTF
*   20          fclose(fp);
 MVC 245(2,#XR1),FP+1
 LA -6,#XR1
 B FCLOSE
*   21          }
 B @CRET
 EXTRN PUTF
 EXTRN _WRITE
 EXTRN _OPEN
 EXTRN FWRITE
 EXTRN FPUTS
 EXTRN FPRINT
 EXTRN FCLOSE
 EXTRN FDOPEN
$42 EQU *
 DC AL2($501)
$22 EQU *
 DC AL2($57)
$02 EQU *
 DC AL2($56)
$61 EQU *
 DC AL2($55)
$41 EQU *
 DC AL2($54)
$21 EQU *
 DC AL2($53)
$01 EQU *
 DC AL2($52)
$6 EQU *
 DC AL2($51)
$4 EQU *
 DC AL2($5)
 EXTRN @CRET
 EXTRN @CENT
 ENTRY FD
FD EQU $DATA
#XR1 EQU X'01'
#XR2 EQU X'02'
#ARR EQU X'08'
#IAR EQU X'10'
#WR4 EQU X'44'
#WR5 EQU X'45'
#WR6 EQU X'46'
#WR7 EQU X'47'
 EXTRN @AC0
 EXTRN @FP
 EXTRN @XR2
 EXTRN @CONST
 END
