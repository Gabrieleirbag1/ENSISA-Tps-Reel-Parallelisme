CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -pthread -fopenmp
LDFLAGS := -pthread -fopenmp

TARGET := dist/main
SOURCES := main.cpp
OBJECTS := $(SOURCES:src/%.cpp=dist/%.o)

.PHONY: all build run clean FORCE

all: build

build: $(TARGET)

$(TARGET): $(OBJECTS) FORCE
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

dist/%.o: src/%.cpp FORCE
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

FORCE: