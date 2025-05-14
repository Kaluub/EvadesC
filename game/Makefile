SRC = src/*.c src/**/*.c
HEADERS = src/*.h src/**/*.h
LIB = -L lib/ -I include/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

default: maps/world.bin debug

maps/world.bin: maps/definitions/regions/*.yaml maps/definitions/world.yaml maps/packer/packer.py
	python3 maps/packer/packer.py

debug:
	make evades EXTRA='-g -DDEBUG'

evades: targets/evades.c $(SRC) $(HEADERS) 
	gcc -o evades targets/evades.c $(SRC) -Wall -Wextra $(LIB) $(EXTRA)

web: targets/evades.c $(SRC) $(HEADERS)
	emcc -o evades.html targets/evades.c $(SRC) -Os -Wall ./lib/libraylib.web.a -I. -I./include -L. -L./lib \
	-DPLATFORM_WEB -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=128mb -s STACK_SIZE=32mb -s ASSERTIONS=1 --preload-file assets \
	--shell-file minshell.html

memory_test: targets/memory_test.c $(SRC) $(HEADERS)
	gcc -o memory_test targets/memory_test.c $(SRC) -g -Wall $(LIB) $(EXTRA)

clean:
	rm -f evades memory_test evades.wasm evades.html evades.data evades.js