#gcc *.c -std=gnu99 -Wall -I../ZDK -L../ZDK -lzdk -lncurses -lm -o a1_n10318399
OBJ=array_management.o debug.o helpers.o main.o program_images.o vacuum.o vector_math.o

CC_FLAGS=-std=gnu99 -Wall -I../ZDK -Wall -Werror -g
LD_FLAGS=-L../ZDK -lzdk -lncurses -lm

a1_n10318399: $(OBJ)
				gcc $(OBJ) $(LD_FLAGS) -o $@
%.o : %.c
		gcc -c $< $(CC_FLAGS)