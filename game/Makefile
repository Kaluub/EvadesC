SRC = src/*.c src/**/*.c src/**/**/*.c
HEADERS = src/*.h src/**/*.h src/**/**/*.h
LINK = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
OUT = evades
ifeq ($(OS), Windows_NT)
	LINK = -DWINDOWS -lraylib -lgdi32 -lwinmm
	OUT = evades.exe
endif
LIB = -L lib/ -I include/ $(LINK)

default: debug

maps/world.bin: maps/definitions/regions/*.yaml maps/definitions/world.yaml maps/packer/packer.py
	python3 maps/packer/packer.py

debug:
	make $(OUT) EXTRA='-g -DDEBUG -fsanitize=undefined'

$(OUT): maps/world.bin targets/evades.c $(SRC) $(HEADERS) 
	gcc -o evades targets/evades.c $(SRC) -Wall -Wextra $(LIB) $(EXTRA)

web: maps/world.bin targets/evades.c $(SRC) $(HEADERS)
	emcc -o evades.html targets/evades.c $(SRC) -Os -Wall -Wextra ./lib/libraylib.web.a -I. -I./include -L. -L./lib \
	-DPLATFORM_WEB -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=128mb -s STACK_SIZE=32mb -s ASSERTIONS=1 --preload-file assets \
	--preload-file maps/world.bin --shell-file minshell.html -DDEBUG

memory_test: targets/memory_test.c $(SRC) $(HEADERS)
	gcc -o memory_test targets/memory_test.c $(SRC) -g -Wall $(LIB) $(EXTRA)

rng_test: targets/rng_test.c src/util/random.c src/util/random.h
	gcc -o rng_test targets/rng_test.c src/util/random.c -g -Wall $(LIB) $(EXTRA)

clean:
	rm -f evades memory_test rng_test evades.wasm evades.html evades.data evades.js evades.zip
ifeq ($(OS), Windows_NT)
	rm -f evades.exe memory_test.exe rng_test.exe
endif

release:
	make $(OUT) EXTRA='-O2'
	zip -r evades.zip $(OUT) maps/world.bin assets