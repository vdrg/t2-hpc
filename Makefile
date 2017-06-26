CC = mpicc
CFLAGS = -c -Wall -g -Os
LD = $(CC)
LDFLAGS = -lm

BUILDDIR = build

TARGET = tsp

DIRS = src src/processes
SOURCES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS)
