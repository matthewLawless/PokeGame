CC = g++
CFLAGS = -Wall
LDFLAGS = -lncurses
OBJFILES = Terrain_Generation.o heap.o
TARGET = TerrainGeneration

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~