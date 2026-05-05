# !/bin/bash
  compiler="/usr/bin/fpc"
  params=" -MObjFPC -Scgi -Cg -O1 -g -gl -l -vewnhibq -Fu/usr/lib64/lazarus/lcl/units/x86_64-linux/gtk2 -Fu/usr/lib64/lazarus/lcl/units/x86_64-linux -Fu/usr/lib64/lazarus/components/freetype/lib/x86_64-linux -Fu/usr/lib64/lazarus/components/lazutils/lib/x86_64-linux -Fu/usr/lib64/lazarus/packager/units/x86_64-linux -Fu.  -dLCL -dLCLgtk2 -Fr/usr/lib64/fpc/msg/errore.msg nevcodeGeany.pas"
  output="nevGeany.pas"
$compiler $params $output
