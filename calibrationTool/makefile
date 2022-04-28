CC = g++

# Directories
HD_DIR = headers
SRC_DIR = src
OBJ_DIR = obj

# Files
SRCS = $(SRC_DIR)/*.cpp

# Objects to be created
OBJS = $(OBJ_DIR)/ButtonsUtils.o $(OBJ_DIR)/AppFrame.o $(OBJ_DIR)/App.o $(OBJ_DIR)/LoadImages.o \
	$(OBJ_DIR)/Mosaic.o $(OBJ_DIR)/ExtractGridCorners.o $(OBJ_DIR)/PreferencesPerspective.o \
	$(OBJ_DIR)/Calibration.o $(OBJ_DIR)/ComputeViewsError.o $(OBJ_DIR)/Save.o $(OBJ_DIR)/LoadFile.o \
	$(OBJ_DIR)/AbstractPreferences.o $(OBJ_DIR)/PreferencesSpherical.o $(OBJ_DIR)/ShowReprojection.o \
	$(OBJ_DIR)/CalibrationResults.o

# Flags
LDFLAGS := -lm
CFLAGS := -Wall

# Windows OS
ifeq ($(OS), Windows_NT)
	RM := del
	EXEC := app.exe
	OBJS += $(OBJ_DIR)/rc.o
	CLEAN_OBJS := /q $(OBJ_DIR)\*.o

# Include path for wxWidgets + path to wx/setup.h (required)
	WXB := -I "C:\Users\sedix\Documents\libs\wxWidgets\lib\gcc810_x64_dll\mswud" \
		-I "C:\Users\sedix\Documents\libs\wxWidgets\include"
# Lib path for wxWidgets + libraries linked
	WXL := -L "C:\Users\sedix\Documents\libs\wxWidgets\lib\gcc810_x64_dll" \
		-lwxmsw316u_core_gcc810_x64 -lwxbase316u_gcc810_x64
# Include path for openCV
	OPENCVB := -I "C:\Users\sedix\Documents\libs\opencv\install\include"
# Lib path for openCV  + libraries linked (shared, must be lib dir for static)
	OPENCVL := -L "C:\Users\sedix\Documents\libs\opencv\install\x64\mingw\bin" \
			-lopencv_highgui455 -lopencv_core455 -lopencv_ccalib455 -lopencv_calib3d455 \
			-lopencv_imgproc455 -lopencv_imgcodecs455

# Other OS (only Linux supported)
else
	RM := rm
	EXEC := app
	CLEAN_OBJS := $(OBJ_DIR)/*.o

# Include path for wxWidgets
	WXB := `wx-config --cxxflags`
# Lib path for wxWidgets
	WXL := `wx-config --libs`
# Include path (+ lib path) for openCV
	OPENCVB := `pkg-config --cflags --libs opencv4`
# Lib path for openCV (remains the same)
	OPENCVL := $(OPENCV)
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(WXL) $(OPENCVL) $(LDFLAGS) -o $(EXEC)

$(OBJ_DIR)/App.o: $(SRC_DIR)/App.cpp $(HD_DIR)/AppFrame.hpp
	$(CC) -c $(WXB) $(OPENCVB) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HD_DIR)/%.hpp
	$(CC) -c $(WXB) $(OPENCVB) $(CFLAGS) -o $@ $<

# Resource file (windows only)
$(OBJ_DIR)/rc.o:
	windres $(WXB) app.rc -o $@

clean:
	$(RM) $(EXEC) $(CLEAN_OBJS)