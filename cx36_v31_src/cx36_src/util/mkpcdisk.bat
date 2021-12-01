rem batch file to format and transfer
rem S/36 files to diskettes
rem
pause insert disk 2 of 3 in drive b:
format b:
label b:whitesmiths
rtopcb cealib.tto
pause insert disk 3 of 3 in drive b:
format b:
label b:whitesmiths
rtopcb calib.tto
