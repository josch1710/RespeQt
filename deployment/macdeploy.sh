#!/bin/bash -x

if [[ "$1" == "" || "$2" == "" || "$3" == "" ]]; then
  echo "Please specify the project directory, the build directory, and the version number";
  exit;
fi

# Build a deployment directory with the correct version number
cd $1
deploydir="$2/RespeQt_$3";

install -d ${deploydir}
cp -aR "$2/RespeQt.app" ${deploydir}/RespeQt.app

#macdeployqt $deploydir/RespeQt.app -always-overwrite

for dir in bootata bootdxl bootmyd bootpic bootsma
do
    install -d "${deploydir}/${dir}"
    install -p "$1/$dir"/* "${deploydir}/${dir}"
done

#for file in atascii_fonts_for_PC.zip atascii_read_me.txt compile\ and\ install.html history.txt license.txt readme.txt RespeQt\ User\ Manual-English.html usb2pc_build_instructions.pdf
#do
#  install -p "$1/$file" ${deploydir}/docs/
#done

install -d "${deploydir}/rcl/spartados"
install "$1/atari/rcl_SpartaDos/"* "${deploydir}/rcl/spartados"
install -d "${deploydir}/rcl/realdos"
install "$1/atari/rcl_RealDos/"* "${deploydir}/rcl/realdos"
install -d "${deploydir}/rcl/menu"
install "$1/atari_8-bit_Menu/"* "${deploydir}/rcl/menu"

cd $2
zip -9r -D "RespeQt_$3" $deploydir
create-dmg --volname "RespeQt_$3" --volicon "$1/RespeQt.icns" --background "$1/main-icon/RespeQt.png" "RespeQt_$3.dmg" "$deploydir"
