echo off
echo TRTOPC>battmp.tto
echo s36 to pc transfer>>battmp.tto
echo FROM        WRKLIB/s/%1>>battmp.tto
echo SELECT      *>>battmp.tto
echo WHERE>>battmp.tto
echo ORDER BY>>battmp.tto
echo 3>>battmp.tto
echo %1.s36>>battmp.tto
echo 1311>>battmp.tto
echo  8011 661>>battmp.tto
echo %1.FDF>>battmp.tto
echo transfer from WRKLIB/s/%1 to %1.s36
rtopcb battmp.tto
del battmp.tto
