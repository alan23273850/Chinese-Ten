CC      = cl
CCFLAGS = /c
OBJ     = main.obj body.obj color.obj card.obj deck.obj player.obj ranking.obj utility.obj
TARGET  = Chinese_Ten.exe
FLAGS	= /EHsc # /W4

everything: clean $(TARGET)

clean:
	del $(OBJ) $(TARGET)

.cpp.obj:
	$(CC) $(CCFLAGS) $< /Fo$@ $(FLAGS)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) /Fe$@
