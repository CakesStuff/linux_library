SOURCES = $(shell find src -type f -name "*.c")
OBJECTS = $(patsubst src/%.c, build/obj/%.o, $(SOURCES))
HEADERS = $(shell find include -type f -name "*.h")

default: all

build:
	mkdir -p $@
build/obj: | build
	mkdir -p $@

build/obj/%.o: src/%.c $(HEADERS) | build/obj
	gcc -Wall -Iinclude -fPIC -g -O2 -c $< -o $@
build/libld.a: $(OBJECTS) | build
	ar rcs $@ $^
build/libld.so: $(OBJECTS) | build
	gcc -shared -Wl,-soname=libld.so $^ -o $@

all: build/libld.a build/libld.so

clean:
	rm -rf build

.PHONY: default clean all