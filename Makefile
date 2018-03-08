CC := gcc
CFLAG := -Wall -Werror -g

all: 
	$(CC) $(CFLAG) -o maxTweeter maxTweeter.c

clean:
	rm -f maxTweeter