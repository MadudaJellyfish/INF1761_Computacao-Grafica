
TARGET = t

all: $(TARGET)

INC = -I. -I/Users/celes/Work/include -I/opt/homebrew/include 
LIB = -L.  -L/opt/homebrew/lib

CPP = CC
#CPPFLAGS = -O2 -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 

SRC = \
  error.cpp \
  shader.cpp \
  triangle.cpp \
  main.cpp

OBJ = $(addsuffix .o, $(basename $(SRC))) 

.cpp.o:
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ) Makefile 
	$(CPP) $(LIB) -o $@ $(OBJ) -lstdc++ -framework OpenGL -lglfw -lassimp


clean:
	rm -f $(TARGET) $(OBJ)


