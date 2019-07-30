OBJS = $(wildcard *.c)
CC = c99
COMPILER_FLAGS = -Wall -pedantic
LINKER_FLAGS = -lSDL2 -lSDL2_image
OBJ_NAME = jandu

#This is the target that compiles our executable
all : $(OBJS)
	mkdir -p build
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OBJ_NAME)

.PHONY: clean
clean :
	rm -rf build
