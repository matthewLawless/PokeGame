CC = gcc
CFLAGS = -Wall
LDFLAGS = 
OBJFILES = Terrain_Generation.o heap.o
TARGET = Terrain_Generation

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~