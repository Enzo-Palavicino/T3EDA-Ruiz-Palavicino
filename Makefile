CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -I./src
LDFLAGS :=

TARGET := EdaCal
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
