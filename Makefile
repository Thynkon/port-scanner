CC = gcc

CFLAGS = -Wall -g

SRCS = *.c

MAIN = port_scanner

all: $(MAIN)
	@echo Port-scanner has been successfully compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(MAIN)

clean:
	$(RM) *~ $(MAIN)
