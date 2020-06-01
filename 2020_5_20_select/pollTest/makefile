all:test test_poll
test:test.cpp
	gcc $^ -o $@;

test_poll:test_poll.cpp
	gcc $^ -o $@;

.PHONY:clean
clean:
	rm -f test;
	rm -f test_poll

