run: test
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a btree.cpp
	./a

test:
	g++ -std=c++14 \
		-o test_runner test_main.cpp btree_test.cpp \
		-I/opt/homebrew/opt/googletest/include \
		-L/opt/homebrew/opt/googletest/lib \
		-lgtest \
		-lgtest_main \
		-pthreads
	./test_runner

run-1ai:
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a prob1ai.cpp
	./a

run-1aii:
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a prob1aii.cpp
	./a

run-1aiii:
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a prob1aiii.cpp
	./a

run-1b:
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a prob1b.cpp
	./a

run-2:
	rm -rf a
	g++ -I../../include -Wall -std=c++14 -o a prob2.cpp
	./a