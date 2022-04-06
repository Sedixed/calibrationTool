CC = g++
RM = rm

EXEC = app
SRCS = src/ButtonsUtils.cpp src/AppFrame.cpp src/App.cpp src/LoadImages.cpp
OBJS = ButtonsUtils.o AppFrame.o App.o LoadImages.o

LIBSWX = `wx-config --cxxflags --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC) # clean

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBSWX) $(LIBSOCV) $(LDFLAGS)

ButtonsUtils.o: src/ButtonsUtils.cpp headers/ButtonsUtils.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

LoadImages.o: src/LoadImages.cpp headers/LoadImages.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

AppFrame.o: src/AppFrame.cpp headers/AppFrame.hpp headers/ButtonsUtils.hpp headers/LoadImages.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

App.o: src/App.cpp headers/AppFrame.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

clean:
	$(RM) *.o $(EXEC)

