mkdir -p game/lib
git clone git@github.com:raysan5/raylib.git
git checkout 305c7a2ef98cf3fa6adde14a3804e1e315994e34
cd raylib/src
make clean
make PLATFORM=PLATFORM_DESKTOP
cp -v libraylib.a ../../game/lib
make clean
make PLATFORM=PLATFORM_WEB
cp -v libraylib.web.a ../../game/lib