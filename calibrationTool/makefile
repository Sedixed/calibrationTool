CC = g++
RM = rm

EXEC = app
SRCS = src/ButtonsUtils.cpp src/AppFrame.cpp src/App.cpp src/LoadImages.cpp src/Mosaic.cpp #src/ExtractGridCorners.cpp
OBJS = ButtonsUtils.o AppFrame.o App.o LoadImages.o Mosaic.o #ExtractGridCorners.o
# AppFrame headers required (there are a lot)
AF_HDS = headers/AppFrame.hpp headers/ButtonsUtils.hpp headers/LoadImages.hpp #headers/ExtractGridCorners.hpp

LIBSWX = `wx-config --cxxflags --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC) # clean

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBSWX) $(LIBSOCV) $(LDFLAGS)

ButtonsUtils.o: src/ButtonsUtils.cpp headers/ButtonsUtils.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

LoadImages.o: src/LoadImages.cpp headers/LoadImages.hpp headers/Mosaic.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

AppFrame.o: src/AppFrame.cpp $(AF_HDS)
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

App.o: src/App.cpp headers/AppFrame.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

Mosaic.o: src/Mosaic.cpp headers/Mosaic.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

#ExtractGridCorners.o: src/ExtractGridCorners.cpp headers/ExtractGridCorners.hpp


clean:
	$(RM) *.o $(EXEC)

