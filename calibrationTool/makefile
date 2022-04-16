CC = g++
RM = rm

EXEC = app
# Source files
SRCS = src/ButtonsUtils.cpp src/AppFrame.cpp src/App.cpp src/LoadImages.cpp src/Mosaic.cpp \
	src/ExtractGridCorners.cpp src/Preferences.cpp src/Calibration.cpp src/CornersReprojection.cpp

# Objects to be created
OBJS = ButtonsUtils.o AppFrame.o App.o LoadImages.o Mosaic.o ExtractGridCorners.o Preferences.o \
	Calibration.o CornersReprojection.o

# AppFrame headers required (there are a lot)
AF_HDS = headers/AppFrame.hpp headers/ButtonsUtils.hpp headers/LoadImages.hpp \
	headers/ExtractGridCorners.hpp headers/Preferences.hpp headers/Calibration.hpp

LIBSWXBUILD = `wx-config --cxxflags`
LIBSWXLINK = `wx-config --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC) # clean

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBSWXLINK) $(LIBSOCV) $(LDFLAGS) -o $(EXEC)

ButtonsUtils.o: src/ButtonsUtils.cpp headers/ButtonsUtils.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

LoadImages.o: src/LoadImages.cpp headers/LoadImages.hpp headers/Mosaic.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

AppFrame.o: src/AppFrame.cpp $(AF_HDS)
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

App.o: src/App.cpp headers/AppFrame.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

Mosaic.o: src/Mosaic.cpp headers/Mosaic.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

ExtractGridCorners.o: src/ExtractGridCorners.cpp headers/ExtractGridCorners.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

Preferences.o: src/Preferences.cpp headers/Preferences.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

Calibration.o: src/Calibration.cpp headers/Calibration.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

CornersReprojection.o: src/CornersReprojection.cpp headers/CornersReprojection.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

clean:
	$(RM) *.o $(EXEC)

