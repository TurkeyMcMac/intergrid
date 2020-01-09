exe = grid
sources = $(wildcard src/*.cpp)
headers = $(wildcard src/*.hpp)

$(exe): $(sources) $(headers)
	c++ -std=c++03 -pedantic -Wall -Wextra $(CXXFLAGS) -o $@ $(sources) -lSDL2

.PHONY: fmt
fmt:
	clang-format -i $(sources) $(headers)

.PHONY: clean
clean:
	rm -f $(exe)
