 OPTIONS NOXREF
*************************************************************************
*  COPYRIGHT (C) 1986, 1987 BY UNISOFT AB, SWEDEN                       *
*                                                                       *
*  @START: calls startup routine                                        *
*                                                                       *
*  NOTE !  this module must be linked first                             *
*                                                                       *
*************************************************************************
*
@START   START 0
*
         PRINT NODATA,NOGEN
*
         EXTRN @CRT36         start up routines
*
         B     @CRT36         call startup routine
*
         END
