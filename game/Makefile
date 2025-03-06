SRC = src/*.c
HEADERS = src/*.h
LIB = -L lib/ -I include/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

default: maps/world.bin debug

maps/world.bin: maps/definitions/regions/*.yaml maps/definitions/world.yaml maps/packer/packer.py
	python3 maps/packer/packer.py

debug:
	make evades EXTRA='-g -DDEBUG'

evades: targets/evades.c $(SRC) $(HEADERS) 
	gcc -o evades targets/evades.c $(SRC) -Wall $(LIB) $(EXTRA)

memory_test: targets/memory_test.c $(SRC) $(HEADERS)
	gcc -o memory_test targets/memory_test.c $(SRC) -g -Wall $(LIB) $(EXTRA)

clean:
	rm -f evades memory_test