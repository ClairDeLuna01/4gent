CC = g++
CPPFLAGS = -Wall -Ofast
LIBFLAGS = -Iinclude
LINKFLAGS = 
INCLUDE = -Iinclude 
EXEC = 4gent
DEL_ux = rm -f
DEL_win = del /Q /F


ODIR=obj
IDIR=include
SDIR=src

SOURCES := $(wildcard $(SDIR)/*.cpp)
OBJ := $(ODIR)/main.o
OBJ += $(SOURCES:$(SDIR)/%.cpp=$(ODIR)/%.o)

# g++ -o hellot.exe main.cpp libglfw3.a libglfw3dll.a -I include -L./ -lglew32 -lglfw3 -lopengl32 

default: $(EXEC)

run :
	$(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(LINKFLAGS) -o $(EXEC) $(LIBFLAGS)

install : $(EXEC)

reinstall : clean install

reinstallwin : cleanwin install

obj/main.o : main.cpp
	$(CC) -c $(CPPFLAGS) $(LIBFLAGS) $(INCLUDE) $< -o $@ 

obj/%.o : src/%.cpp
	$(CC) -c $(CPPFLAGS) $(LIBFLAGS) $(INCLUDE) $< -o $@ 

PHONY : clean

cleanwin : 
	$(DEL_win) $(EXEC) obj\*.o

clean : 
	$(DEL_ux) $(EXEC) obj\*.o

countlines :
	find ./ -type f \( -iname \*.cpp -o -iname \*.hpp -o -iname \*.frag -o -iname \*.vert -o -iname \*.geom \) | sed 's/.*/"&"/' | xargs  wc -l