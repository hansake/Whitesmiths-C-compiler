*    FLOATING CONSTANTS - IBM/370, IBM S/36 STYLE
*    copyright (c) 1983, 1986 by Whitesmiths, Ltd.
*
_DTENS   START
*
         ENTRY _DTENS
         ENTRY _DZERO
         ENTRY _MAXPR
         ENTRY _NTENS
*
         DC FL8'1E0'
         DC FL8'1E1'
         DC FL8'1E2'
         DC FL8'1E4'
         DC FL8'1E8'
         DC FL8'1E16'
         DC FL8'1E32'
         DC FL8'1E64'
_DZERO   EQU *
         DC FL8'0'
_MAXPR   EQU *
         DC IL2'15'
_NTENS   EQU *
         DC IL2'8'
*
         END
