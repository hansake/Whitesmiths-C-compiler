echo off
if exist %1.ASM goto doit
echo %1.asm not found
goto next
: doit
echo TRFRPC>battmp.tfr
echo send %1.ASM to WRKLIB/S/%1>>battmp.tfr
echo TO          WRKLIB/S/%1>>battmp.tfr
echo %1.ASM>>battmp.tfr
echo 21>>battmp.tfr
echo %1.FDF>>battmp.tfr
echo send %1.ASM to WRKLIB/S/%1
rfrompcb battmp.tfr > tfrlog.tmp
if not errorlevel 20 goto next
type tfrlog.tmp
goto end
: next
shift
if %1xxx == xxx goto end
towrk36 %1 %2 %3 %4 %5 %6 %7 %8 %9
: end
echo end of transfer
if not exist tfrlog.tmp goto exit
del tfrlog.tmp
: exit
del battmp.tfr
