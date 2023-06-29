ifeq ($(wildcard project.local.makefile),)
	# ignore
else
    include project.local.makefile
endif

CC = gcc
BUILDDIR = build
CFLAGS += -std=c11
LDFLAGS += 
SOURCES = $(filter-out test_%.c,$(wildcard *.c))
OBJECTS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
LIBS = -lcurl -ljson-c -lreadline
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CFLAGS += -DLINUX
		LDFLAGS +=
else ifeq ($(UNAME_S),Darwin)
		CFLAGS += -I /opt/homebrew/include/ -DMACOS
		LDFLAGS += -L /opt/homebrew/lib
else
    $(error Unsupported operating system: $(UNAME_S))
endif

.PHONY: all clean

all: $(BUILDDIR)/main

$(BUILDDIR)/main: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/main $(OBJECTS)
