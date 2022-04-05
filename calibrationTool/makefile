CC = g++
RM = rm

EXEC = app
SRCS = appFrame.cpp app.cpp 
OBJS = appFrame.o app.o

LIBSWX = `wx-config --cxxflags --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`
LDFLAGS = -lm

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBSWX) $(LIBSOCV) $(LDFLAGS)

appFrame.o: appFrame.cpp appFrame.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

app.o: app.cpp appFrame.hpp
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

clean:
	$(RM) *.o $(EXEC)

