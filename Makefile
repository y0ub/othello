cc_version = -std=c++17
target     = othello

$(target): bin/board.o
	clang++ -Wall $(cc_version) -o bin/$@ src/main.cc $^

board.o:
	clang++ -c -Wall $(cc_version) -o bin/$@ src/board.cc

run:
	./bin/$(target)
