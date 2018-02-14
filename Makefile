# Program, flags, etc.
CC      = cl
CCFLAGS = /c
OBJ     = main.obj color.obj card.obj deck.obj computer.obj player.obj ranking.obj
TARGET  = demo.exe
FLAGS	= /W4 /EHsc

everything: $(TARGET)

clean:
	del $(OBJ) $(TARGET)

all: clean everything

.cpp.obj:
	$(CC) $(CCFLAGS) $< /Fo$@ $(FLAGS)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) /Fe$@