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

.PHONY: release debug web clean

maps/world.bin: maps/definitions/regions/*.yaml maps/definitions/world.yaml maps/packer.py
	python3 maps/packer.py

debug:
	make $(OUT) EXTRA='-g -DDEBUG -fsanitize=undefined'

$(OUT): targets/evades.c $(SRC) $(HEADERS)
	make maps/world.bin
	gcc -o evades targets/evades.c $(SRC) -Wall -Wextra $(LIB) $(EXTRA)

web: maps/world.bin targets/evades.c $(SRC) $(HEADERS)
	emcc -o index.html targets/evades.c $(SRC) -Os -Wall -Wextra ./lib/libraylib.web.a -I. -I./include -L. -L./lib \
		-DPLATFORM_WEB -s USE_GLFW=3 -s TOTAL_MEMORY=128mb -s STACK_SIZE=32mb -s ASSERTIONS=1 \
		-sEXPORTED_FUNCTIONS=_main,_resize_game -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
		--preload-file assets --preload-file maps/world.bin --shell-file shell.html -DDEBUG
	mkdir -p release/web
	mv -v index.* release/web

memory_test: targets/memory_test.c $(SRC) $(HEADERS)
	gcc -o memory_test targets/memory_test.c $(SRC) -g -Wall $(LIB) $(EXTRA)

rng_test: targets/rng_test.c src/util/random.c src/util/random.h
	gcc -o rng_test targets/rng_test.c src/util/random.c -g -Wall $(LIB) $(EXTRA)

clean:
	rm -f evades memory_test rng_test
ifeq ($(OS), Windows_NT)
	rm -f evades.exe memory_test.exe rng_test.exe
endif

release:
	make $(OUT) EXTRA='-O2'
	mkdir -p release/native
	zip -r release/native/evades.zip $(OUT) maps/world.bin assets