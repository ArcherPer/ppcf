CC = gcc
CPATH = -Iinclude
CFLAGS = -Wall -Werror

SRCDIRS := $(shell find . -type d)
SRCFILES := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

OBJGLOB := ppcf.obj
STATICLIB := libppcf.a
SHAREDLIB := libppcf.so

.PHONY: all clean

all: $(OBJGLOB) $(SHAREDLIB) $(STATICLIB)

$(OBJGLOB): $(OBJFILES)
	$(CC) $(CPATH) $(CFLAGS) -shared -o $@ $^

$(SHAREDLIB): $(OBJFILES)
	$(CC) $(CPATH) $(CFLAGS) -shared -o $@ $^

$(STATICLIB): $(OBJGLOB)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CPATH) $(CFLAGS) -shared -c -o $@ $^

clean:
	rm -f $(OBJGLOB) $(STATICLIB) $(SHAREDLIB) $(OBJFILES)