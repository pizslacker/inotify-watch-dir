inotify-watch-dir: inotify-watch-dir.c
	gcc -Wall -Wextra -O2 -o inotify-watch-dir inotify-watch-dir.c.c
	strip inotify-watch-dir

clean:
	rm -f inotify-watch-dir
