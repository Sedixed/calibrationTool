# DEBUG or RELEASE : used for Linux OS
# if RELEASE is set, the path to the documentation will be usr/share/calibrationToolDoc/
# if DEBUG is set, the path will be docs (from the current directory)
VERSION_TYPE = RELEASE

CC = g++

# Directories
HD_DIR = headers
SRC_DIR = src
OBJ_DIR = obj
RESOURCE_DIR = resource

WXB_DIR ?= include/wx
OPENCVB_DIR ?= include/opencv
WXL_DIR ?= libs/wx
OPENCVL_DIR ?= libs/opencv
WXSETUP_DIR ?= $(WXB_DIR)


# Files
SRCS = $(SRC_DIR)/*.cpp

# Objects to be created
OBJS = $(OBJ_DIR)/ButtonsUtils.o $(OBJ_DIR)/AppFrame.o $(OBJ_DIR)/App.o $(OBJ_DIR)/LoadImages.o      \
	$(OBJ_DIR)/Mosaic.o $(OBJ_DIR)/ExtractGridCorners.o $(OBJ_DIR)/PreferencesPerspective.o          \
	$(OBJ_DIR)/Calibration.o $(OBJ_DIR)/Save.o $(OBJ_DIR)/LoadFile.o                                 \
	$(OBJ_DIR)/AbstractPreferences.o $(OBJ_DIR)/PreferencesSpherical.o $(OBJ_DIR)/ShowReprojection.o \
	$(OBJ_DIR)/CalibrationResults.o $(OBJ_DIR)/ImageUtils.o

# Flags
LDFLAGS := -lm
CFLAGS := -Wall
EXEC := calibrationTool

# Windows OS
ifeq ($(OS), Windows_NT)
	RM := del
	CFLAGS += -DWINDOWS
	LDFLAGS += -mwindows
	EXEC_NAME := "Calibration Tool.exe"
	OBJS += $(OBJ_DIR)/resource.o
	CLEAN_OBJS := /q $(OBJ_DIR)\*.o

# Include path for wxWidgets + path to wx/setup.h (required)
	WXB := -I $(WXB_DIR) -I $(WXSETUP_DIR)

# Lib path for wxWidgets + libraries linked
	WXL := -L $(WXL_DIR) \
		-lwxmsw316u_core_gcc810_x64 -lwxbase316u_gcc810_x64

# Include path for OpenCV
	OPENCVB := -I $(OPENCVB_DIR)

# Lib path for openCV  + libraries linked (shared, must be lib dir for static)
	OPENCVL := -L $(OPENCVL_DIR) \
			-lopencv_highgui455 -lopencv_core455 -lopencv_ccalib455 -lopencv_calib3d455 \
			-lopencv_imgproc455 -lopencv_imgcodecs455

# Other OS (only Linux supported)
else
	RM := rm
	EXEC_NAME := calibrationTool

# Used for the path to documentation
	ifeq ($(VERSION_TYPE), DEBUG)
		CFLAGS += -DDEBUG
	else
		CFLAGS += -DRELEASE
	endif

	CLEAN_OBJS := $(OBJ_DIR)/*.o

# Include path for wxWidgets
	WXB := `wx-config --cxxflags`
# Lib path for wxWidgets
	WXL := `wx-config --libs`
# Include path (+ lib path) for openCV
	OPENCVB := `pkg-config --cflags --libs opencv4`
# Lib path for openCV (remains the same)
	OPENCVL := $(OPENCVB)
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ $(WXL) $(OPENCVL) $(LDFLAGS) -o $(EXEC_NAME)

$(OBJ_DIR)/App.o: $(SRC_DIR)/App.cpp $(HD_DIR)/AppFrame.hpp
	$(CC) -c $(WXB) $(OPENCVB) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HD_DIR)/%.hpp
	$(CC) -c $(WXB) $(OPENCVB) $(CFLAGS) -o $@ $<

# Resource file (windows only)
$(OBJ_DIR)/resource.o:
	windres $(WXB) $(RESOURCE_DIR)/resource.rc -o $@

# Removes the executable and the object files
clean:
	$(RM) $(EXEC_NAME) $(CLEAN_OBJS)

# Removes the object files
clean_objs:
	$(RM) $(CLEAN_OBJS)