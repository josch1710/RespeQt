#!/bin/bash

if [[ "$1" == "" || "$2" == "" || "$3" == "" ]]; then
  echo "Please specify the project directory, the build directory, and the version number";
  exit;
fi

# Build a deployment directory with the correct version number
deploydir="$1/deployment/RespeQt_$3";
echo $deploydir;
mkdir -p $deploydir;
cd $deploydir
cp -aR "$2/RespeQt.app" .
~/Qt/5.6.3/clang_64/bin/macdeployqt RespeQt.app -always-overwrite -verbose=2
mkdir -p "\$boot"
for dir in \$bootata \$bootdxl \$bootmyd \$bootpic \$bootsma
do
  cp -aR "$1/$dir" "\$boot"
done
for file in atascii_fonts_for_PC.zip atascii_read_me.txt compile\ and\ install.html history.txt license.txt readme.txt RespeQt\ User\ Manual-English.html usb2pc_build_instructions.pdf
do
  cp -a "$1/$file" .
done

mkdir -p "rcl"
cp "$1/atari/rcl_SpartaDos/rcl.com" rcl/rclspard.com
cp "$1/atari/rcl_RealDos/rcl.com" rcl/rclreald.com
cp "$1/atari_8-bit_Menu/"*.car rcl/

cd ..
zip -9r -D "RespeQt_$3" $deploydir