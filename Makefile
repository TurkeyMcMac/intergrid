exe = intergrid
sources = src/*.cpp
headers = src/*.hpp

$(exe): $(sources) $(headers)
	$(CXX) -std=c++03 -pedantic -Wall -Wextra $(CXXFLAGS) -o $@ $(sources) -lSDL2

.PHONY: fmt
fmt:
	clang-format -i $(sources) $(headers)

.PHONY: clean
clean:
	rm -f $(exe)
