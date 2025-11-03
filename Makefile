#  	Ethan Silo
# 	z1838047
# 	CSCI 340-PE1

# 	I certify that this is my own work and where appropriate an extension
# 	of the starter code provided for the assignment.
CXX = g++
CXXFLAGS = -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14
CXXFLAGS += -MMD -MP

TARGET = memsim
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
DEPS = $(patsubst %.cpp, %.d, $(SOURCES))


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

re: clean all

-include $(DEPS)
