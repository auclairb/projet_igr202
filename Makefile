EXE = main
SRCS =  Camera.cpp Light.cpp LightTree.cpp Main.cpp Mesh.cpp Ray.cpp Lightcut.cpp
LIBS =  -lglut -lGLU -lGL -lm 
CC = g++
CPP = g++
FLAGS = -Wall -O2 -std=gnu++11
CFLAGS = $(FLAGS)
CXXFLAGS = $(FLAGS)
OBJS = $(SRCS:.cpp=.o)
.PHONY = clean

$(EXE): $(OBJS)
	g++ $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBS)

-include $(subst .c,.d,$(SOURCES))
%.d : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM -MF $@ -MP $<

clean:
	- rm -f  *~  $(EXE) $(OBJS) *.d

