CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = programa_test

DEPS = Solucion1.cpp Solucion2.cpp D1.txt D2.txt

SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)


clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)