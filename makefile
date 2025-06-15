CC = gcc
CFLAGS = -Wall -g

SRCS = main.c agent.c coordinator.c
OBJS = $(SRCS:.c=.o)
TARGET = koordynator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
