CC = g++

NAME = loadtry
SRCS = loadtry.cpp
OBJS = loadtry.o

LIBSWX = `wx-config --cxxflags --libs`
LIBSOCV = `pkg-config --cflags --libs opencv4`

LDFLAGS = -lm


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LIBSWX) $(LIBSOCV) $(LDFLAGS)

$(OBJS): $(SRCS)
	$(CC) -c $(LIBSWX) $(LIBSOCV) -o $@ $<

clean:
	rm *.o $(NAME)



