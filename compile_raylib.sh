mkdir -p game/lib
git clone git@github.com:raysan5/raylib.git
cd raylib
git pull
git checkout 9f831428e6be0eba8762a154e3e9139d4f071970
cd src
make clean
make PLATFORM=PLATFORM_DESKTOP
cp -v libraylib.a ../../game/lib
make clean
make PLATFORM=PLATFORM_WEB
cp -v libraylib.web.a ../../game/lib
cp -v raylib.h raymath.h rcamera.h rlgl.h ../../game/include