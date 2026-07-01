CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++23 -I ./rpi-rgb-led-matrix/include
LDFLAGS = -L ./rpi-rgb-led-matrix/lib -lrgbmatrix -lX11 -lpthread

TARGET = xvfb-grab
SRCS = xvfb-grab.cpp
SUBMODULE_DIR = ./rpi-rgb-led-matrix

all: rpi-rgb-led-matrix $(TARGET)

rpi-rgb-led-matrix:
	$(MAKE) -C $(SUBMODULE_DIR)

$(TARGET): $(SRCS) | rpi-rgb-led-matrix
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	$(MAKE) -C $(SUBMODULE_DIR) clean

.PHONY: all clean rpi-rgb-led-matrix