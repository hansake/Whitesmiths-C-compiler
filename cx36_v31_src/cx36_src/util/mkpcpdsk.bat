rem batch file to format and transfer Pascal
rem S/36 files to diskettes
rem
pause insert Pascal disk drive b:
format b:
label b:whitesmiths
rtopcb cplib.tto
