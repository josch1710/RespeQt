#!/bin/bash -x

if [[ "$1" == "" || "$2" == "" || "$3" == "" ]]; then
  echo "Please specify the project directory, the build directory, and the version number";
  exit;
fi

# Build a deployment directory with the correct version number
cd "$1" || exit 1
longversion=$3
if [[ "$4" != "" ]]; then
  longversion="${longversion}_$4"
fi
deploydir="$2/RespeQt_v${longversion}"

install -d "${deploydir}"
rm -rf "${deploydir}"/RespeQt.app
cp -aR "$2/RespeQt.app" "${deploydir}"
macdeployqt "$deploydir"/RespeQt.app -always-overwrite

install -p "$1/deployment/Info.plist.app" "${deploydir}"/RespeQt.app/Contents/Info.plist
sed -i '' -e "s/@SHORTVERSION@/$3/g" "${deploydir}"/RespeQt.app/Contents/Info.plist
sed -i '' -e "s/@LONGVERSION@/${longversion}/g" "${deploydir}"/RespeQt.app/Contents/Info.plist
install -p "$2"/*.qm "${deploydir}"/RespeQt.app/Contents/Resources/

for dir in bootata bootdxl bootmyd bootpic bootsma
do
    install -d "${deploydir}/RespeQt.app/Contents/Resources/${dir}"
    install -p "$1/$dir"/* "${deploydir}/RespeQt.app/Contents/Resources/${dir}"
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

cd "$2" || exit 2
test -f "RespeQt_${longversion}.zip" && rm "RespeQt_${longversion}.zip"
zip -9r -D "RespeQt_${longversion}.zip" $(basename "${deploydir}")
test -f "RespeQt_${longversion}.dmg" && rm "RespeQt_${longversion}.dmg"

/opt/homebrew/bin/create-dmg --volname "RespeQt_${longversion}" \
    --icon-size 32 \
    --volicon "$1/resources/RespeQt.icns" \
    --eula "$1/license.txt" \
    --no-internet-enable \
    "RespeQt_${longversion}.dmg" "${deploydir}"
