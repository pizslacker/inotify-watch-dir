c-linux-include: c-linux-include.c
	gcc -Wall -Wextra -O2 -o c-linux-include c-linux-include.c
	strip c-linux-include

clean:
	rm -f c-linux-include
