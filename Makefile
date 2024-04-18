EXEC=main
GCC=gcc
OBJ=main.o lrc.o start_mplayer.o console.o background.o
FLAGS=-Wall
$(EXEC):$(OBJ)
	$(GCC) $^ -o $@ $(FLAGS)
%.o:%.c
	$(GCC) -c $< -o $@ $(FLAGS)
clean:
	rm $(EXEC) $(OBJ)