CC = g++
RM = rm

EXEC = app
HD_DIR = headers
SRC_DIR = src
OBJ_DIR = obj
# Source files
SRCS = $(SRC_DIR)/ButtonsUtils.cpp $(SRC_DIR)/AppFrame.cpp $(SRC_DIR)/App.cpp $(SRC_DIR)/LoadImages.cpp \
	$(SRC_DIR)/Mosaic.cpp $(SRC_DIR)/ExtractGridCorners.cpp $(SRC_DIR)/PreferencesPerspective.cpp \
	$(SRC_DIR)/Calibration.cpp $(SRC_DIR)/CornersReprojection.cpp $(SRC_DIR)/AbstractPreferences.cpp \
	$(SRC_DIR)/PreferencesSpherical.cpp

# Objects to be created
OBJS = $(OBJ_DIR)/ButtonsUtils.o $(OBJ_DIR)/AppFrame.o $(OBJ_DIR)/App.o $(OBJ_DIR)/LoadImages.o \
	$(OBJ_DIR)/Mosaic.o $(OBJ_DIR)/ExtractGridCorners.o $(OBJ_DIR)/PreferencesPerspective.o \
	$(OBJ_DIR)/Calibration.o $(OBJ_DIR)/ComputeViewsError.o $(OBJ_DIR)/Save.o $(OBJ_DIR)/LoadFile.o \
	$(OBJ_DIR)/AbstractPreferences.o $(OBJ_DIR)/PreferencesSpherical.o

# AppFrame headers required (there are a lot)
AF_HDS = $(HD_DIR)/AppFrame.hpp $(HD_DIR)/ButtonsUtils.hpp $(HD_DIR)/LoadImages.hpp \
	$(HD_DIR)/ExtractGridCorners.hpp $(HD_DIR)/PreferencesPerspective.hpp $(HD_DIR)/Calibration.hpp \
	$(HD_DIR)/PreferencesSpherical.hpp

LIBSWXBUILD = `wx-config --cxxflags`
LIBSWXLINK = `wx-config --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC) # clean

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBSWXLINK) $(LIBSOCV) $(LDFLAGS) -o $(EXEC)

$(OBJ_DIR)/AppFrame.o: $(SRC_DIR)/AppFrame.cpp $(AF_HDS)
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

$(OBJ_DIR)/App.o: $(SRC_DIR)/App.cpp $(HD_DIR)/AppFrame.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HD_DIR)/%.hpp
	$(CC) -c $(LIBSWXBUILD) $(LIBSOCV) -o $@ $<

clean:
	$(RM) $(OBJ_DIR)/*.o $(EXEC)

