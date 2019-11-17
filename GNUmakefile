CC = g++
CFLAGS = -MD # -Wall
EXE = Chinese_Ten.out
OBJS = body.o card.o color.o deck.o main.o player.o ranking.o utility.o

all:$(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE) *.o *.d

-include $(OBJS:.o=.d)
