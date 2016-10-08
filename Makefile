CC = gcc
CFLAGS = -g -O0 -Wall

all: rbtree_test

rbtree_test: rbtree_test.o rbtree_test.o dmodule.o rbtree.o
	@echo [link]
	$(CC) $(CFLAGS) $^ -o $@

rbtree_test.o: rbtree_test.c
	@echo [compile]
	$(CC) $(CFLAGS) -c $^ -o $@

dmodule.o: dmodule.c dmodule.h
	@echo [compile]
	$(CC) $(CFLAGS) -c dmodule.c -o $@

rbtree.o: rbtree.c rbtree.h
	@echo [compile]
	$(CC) $(CFLAGS) -c rbtree.c -o $@

.PHONY: clean

clean:
	@echo [clean]
	@rm -f rbtree_test rbtree_test.o dmodule.o rbtree.o
