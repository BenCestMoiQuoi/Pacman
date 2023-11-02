CC = g++
CPPFLAGS = -std=c++11 -c -g
LD = g++
LDFLAGS = -std=c++11
FINAL_TARGET = pacman

Qt_DIR = libs/QtBase
INCLUDE_DIR = -I$(Qt_DIR)/include
LIB_DIR = -L$(Qt_DIR)/lib
LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

default: $(FINAL_TARGET)

$(FINAL_TARGET): jeu.o main.o pacmanwindow.o
	$(LD) $+ -o $@ $(LDFLAGS) $(LIB_DIR) $(LIBS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE_DIR) $< -o $@

clean:
	del *.o $(FINAL_TARGET)