CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Iinclude -Wall -O2 -Werror -Wextra -pedantic

all: out.exe

%.o: %.c
	gcc $(GCCFLAGS) -c $^ -o $@

out.exe: $(OFILES)
	gcc $(GCCFLAGS) $^ -o $@

clean:
	del /S *.o