CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -w
SOURCES = main.cpp catan.cpp board.cpp player.cpp card.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = catan_game

TEST_SOURCES = tests.cpp catan.cpp board.cpp player.cpp card.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
TEST_EXECUTABLE = run_tests

all: $(EXECUTABLE) $(TEST_EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE)

main: $(EXECUTABLE)
	./$(EXECUTABLE)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

.PHONY: all clean main test