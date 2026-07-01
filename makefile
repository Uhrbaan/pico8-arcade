CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++23 -I ./rpi-rgb-led-matrix/include
LDFLAGS = -L ./rpi-rgb-led-matrix/lib -lrgbmatrix -lX11 -lpthread

TARGET = xvfb-grab
SRCS = xvfb-grab.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean