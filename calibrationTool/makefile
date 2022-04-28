CC = g++

EXEC = app
HD_DIR = headers
SRC_DIR = src
LIBS_DIR = "libs"
OBJ_DIR = obj
# Source files
SRCS = $(SRC_DIR)/ButtonsUtils.cpp $(SRC_DIR)/AppFrame.cpp $(SRC_DIR)/App.cpp $(SRC_DIR)/LoadImages.cpp \
	$(SRC_DIR)/Mosaic.cpp $(SRC_DIR)/ExtractGridCorners.cpp $(SRC_DIR)/PreferencesPerspective.cpp \
	$(SRC_DIR)/Calibration.cpp $(SRC_DIR)/CornersReprojection.cpp $(SRC_DIR)/AbstractPreferences.cpp \
	$(SRC_DIR)/PreferencesSpherical.cpp $(SRC_DIR)/ShowReprojection.cpp $(SRC_DIR)/CalibrationResults.cpp

# Objects to be created
OBJS = $(OBJ_DIR)/ButtonsUtils.o $(OBJ_DIR)/AppFrame.o $(OBJ_DIR)/App.o $(OBJ_DIR)/LoadImages.o \
	$(OBJ_DIR)/Mosaic.o $(OBJ_DIR)/ExtractGridCorners.o $(OBJ_DIR)/PreferencesPerspective.o \
	$(OBJ_DIR)/Calibration.o $(OBJ_DIR)/ComputeViewsError.o $(OBJ_DIR)/Save.o $(OBJ_DIR)/LoadFile.o \
	$(OBJ_DIR)/AbstractPreferences.o $(OBJ_DIR)/PreferencesSpherical.o $(OBJ_DIR)/ShowReprojection.o \
	$(OBJ_DIR)/CalibrationResults.o

# AppFrame headers required (there are a lot)
AF_HDS = $(HD_DIR)/AppFrame.hpp $(HD_DIR)/ButtonsUtils.hpp $(HD_DIR)/LoadImages.hpp \
	$(HD_DIR)/ExtractGridCorners.hpp $(HD_DIR)/PreferencesPerspective.hpp $(HD_DIR)/Calibration.hpp \
	$(HD_DIR)/PreferencesSpherical.hpp $(HD_DIR)/ShowReprojection.hpp $(HD_DIR/CalibrationResults.hpp)

LDFLAGS := -lm
CFLAGS := -Wall

ifeq ($(OS), Windows_NT)
	RM := del

	WXB := -I "C:\Users\sedix\Documents\libs\wxWidgets\lib\gcc810_x64_dll\mswud" \
		-I "C:\Users\sedix\Documents\libs\wxWidgets\include"

	WXL := -L "C:\Users\sedix\Documents\libs\wxWidgets\lib\gcc810_x64_dll" \
		-lwxmsw316u_core_gcc810_x64 -lwxbase316u_gcc810_x64

	OPENCV := -I "C:\Users\sedix\Documents\libs\opencv\install\include"

	OCVLINK := -L "C:\Users\sedix\Documents\libs\opencv\install\x64\mingw\bin" \
			-L "C:\Users\sedix\Documents\libs\opencv\install\x64\mingw\lib" \
			-lopencv_highgui455 -lopencv_core455 -lopencv_ccalib455 -lopencv_calib3d455 \
			-lopencv_imgproc455 -lopencv_imgcodecs455
else
	RM := rm
	WXB := `wx-config --cxxflags`
	WXL := `wx-config --libs`
	OPENCV := `pkg-config --cflags --libs opencv4`
	OCVLINK := $(OPENCV)
	
endif

all: $(EXEC) #clean

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) $(WXL) $(OCVLINK) $(LDFLAGS) -o $(EXEC) 

$(OBJ_DIR)/AppFrame.o: $(SRC_DIR)/AppFrame.cpp $(AF_HDS)
	$(CC) -c $(WXB) $(OPENCV) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/App.o: $(SRC_DIR)/App.cpp $(HD_DIR)/AppFrame.hpp
	$(CC) -c $(WXB) $(OPENCV) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HD_DIR)/%.hpp
	$(CC) -c $(WXB) $(OPENCV) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJ_DIR)/*.o $(EXEC)