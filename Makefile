CC = gcc
CFLAGS = -g -O0 -Wall

all: rbtree_test

rbtree_test: rbtree_test.o rbtree.o
	@echo [link]
	$(CC) $(CFLAGS) $^ -o $@

rbtree.o: rbtree.c rbtree.h
	@echo [compile]
	$(CC) $(CFLAGS) -c rbtree.c -o $@

rbtree_test.o: rbtree_test.c
	@echo [compile]
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	@echo [clean]
	@rm -f rbtree_test rbtree.o rbtree_test.o
