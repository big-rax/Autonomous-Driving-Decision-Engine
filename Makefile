CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
INCLUDES = -Iinclude

SRC = src/DecisionMaker.cpp src/Vehicle.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

all: av_sim

av_sim: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c main.cpp -o main.o

clean:
	rm -f $(OBJ) av_sim main.o
