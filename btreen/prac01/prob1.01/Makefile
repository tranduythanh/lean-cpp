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