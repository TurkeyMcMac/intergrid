exe = intergrid
sources = src/*.cpp
headers = src/*.hpp

$(exe): $(sources) $(headers)
	$(CXX) -std=c++98 -pedantic -Wall -Wextra -D_REENTRANT $(CXXFLAGS) -o $@ $(sources) -lSDL2

.PHONY: fmt
fmt:
	clang-format -i $(sources) $(headers)

.PHONY: clean
clean:
	rm -f $(exe)
