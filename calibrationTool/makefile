CC = g++
RM = rm

EXEC = app
SRCS = ButtonsUtils.cpp AppFrame.cpp App.cpp
OBJS = ButtonsUtils.o AppFrame.o App.o

LIBSWX = `wx-config --cxxflags --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBSWX) $(LIBSOCV) $(LDFLAGS)

ButtonsUtils.o: ButtonsUtils.cpp ButtonsUtils.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

AppFrame.o: AppFrame.cpp AppFrame.hpp ButtonsUtils.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

App.o: App.cpp AppFrame.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

clean:
	$(RM) *.o $(EXEC)

