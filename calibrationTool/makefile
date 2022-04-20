CC = g++
RM = rm

EXEC = app
# Source files
SRCS = src/ButtonsUtils.cpp src/AppFrame.cpp src/App.cpp src/LoadImages.cpp src/Mosaic.cpp \
	src/ExtractGridCorners.cpp src/PreferencesPerspective.cpp src/Calibration.cpp src/CornersReprojection.cpp

# Objects to be created
OBJS = ButtonsUtils.o AppFrame.o App.o LoadImages.o Mosaic.o ExtractGridCorners.o PreferencesPerspective.o \
	Calibration.o ComputeViewsError.o Save.o LoadFile.o

# AppFrame headers required (there are a lot)
AF_HDS = headers/AppFrame.hpp headers/ButtonsUtils.hpp headers/LoadImages.hpp \
	headers/ExtractGridCorners.hpp headers/PreferencesPerspective.hpp headers/Calibration.hpp

LIBSWXBUILD = `wx-config --cxxflags`
LIBSWXLINK = `wx-config --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC) # clean

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBSWXLINK) $(LIBSOCV) $(LDFLAGS) -o $(EXEC)

AppFrame.o: src/AppFrame.cpp $(AF_HDS)
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

App.o: src/App.cpp headers/AppFrame.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<


%.o: src/%.cpp headers/%.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

clean:
	$(RM) *.o $(EXEC)

