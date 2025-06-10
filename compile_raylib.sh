mkdir -p game/lib
git clone git@github.com:raysan5/raylib.git
git checkout cb3168a0486edd0b1b6a2595ac73719ee0a4b596
cd raylib/src
make clean
make PLATFORM=PLATFORM_DESKTOP
cp -v libraylib.a ../../game/lib
make clean
make PLATFORM=PLATFORM_WEB
cp -v libraylib.web.a ../../game/lib