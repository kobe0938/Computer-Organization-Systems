# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = stack factorial channel buggy smash function_pointers

all:: $(PROGRAMS)

CC = gcc
CFLAGS = -g3 -Og -std=gnu99 -Wall $$warnflags -fno-stack-protector -fcf-protection=none -fno-pic -no-pie
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -Winit-self -fno-diagnostics-show-option
export nofortify = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0
LDFLAGS =
LDLIBS =

$(PROGRAMS): %:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

factorial: CFLAGS += -Og

stack: CFLAGS += -fno-builtin $$nofortify

smash: CFLAGS += -O0 -fomit-frame-pointer

buggy: CFLAGS += -O0 -fomit-frame-pointer

clean::
	rm -f $(PROGRAMS) *.o

.PHONY: clean all
