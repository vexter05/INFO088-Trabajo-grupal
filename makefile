CXX = g++
CXXFLAGS = -Wall -std=c++11 -O3 -march=native

TARGET = programa_test
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)


clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)