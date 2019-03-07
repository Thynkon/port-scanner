CC = gcc

CFLAGS = -Wall -g

SRCS = *.c

OBJS = $(SRCS:.c=.o)

MAIN = port_scanner

all: $(MAIN)
	@echo Port-scanner has been successfully compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

clean:
	$(RM) *.o *~ $(MAIN)
