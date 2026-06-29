CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O3 -I./rpi-rgb-led-matrix/include
LDFLAGS = -L./rpi-rgb-led-matrix/lib -lrgbmatrix -lrt -lm -lpthread

TARGET = pico8arcade
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean