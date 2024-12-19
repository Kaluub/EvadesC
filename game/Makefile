SRC = src/*.c src/**/*.c
HEADERS = src/*.h src/**/*.h
LIB = -L lib/ -I include/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
OUT = evades

default: $(OUT)

debug:
	make $(OUT) EXTRA=-DDEBUG

$(OUT): $(SRC) $(HEADERS)
	gcc $(SRC) -Wall -o $(OUT) $(LIB) $(EXTRA)

clean:
	rm -f $(OUT)