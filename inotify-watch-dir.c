#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h> /* The Linux-specific "import" */
#include <limits.h>

/* Buffer size to hold the inotify events */
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main() {
    int fd, wd;
    char buffer[BUF_LEN];

    // 1. Initialize the inotify instance (Linux-specific system call)
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init failed");
        return EXIT_FAILURE;
    }

    // 2. Add a watch to the current directory (".") 
    // We want to be notified when files are created, deleted, or modified
    wd = inotify_add_watch(fd, ".", IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        perror("inotify_add_watch failed");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Watching the current directory for changes...\n");
    printf("Open another terminal and try running: touch demo_file.txt\n\n");

    // 3. Read events (This is a blocking call; the program pauses here until an event occurs)
    int length = read(fd, buffer, BUF_LEN);
    if (length < 0) {
        perror("read failed");
        return EXIT_FAILURE;
    }

    // 4. Process the events returned by the kernel
    int i = 0;
    while (i < length) {
        struct inotify_event *event = (struct inotify_event *) &buffer[i];
        
        if (event->len) { // Check if the event has an associated file name
            if (event->mask & IN_CREATE) {
                printf("Event: File '%s' was created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("Event: File '%s' was deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("Event: File '%s' was modified.\n", event->name);
            }
        }
        
        // Move the index to the next event in the buffer
        i += sizeof(struct inotify_event) + event->len;
    }

    // 5. Clean up Linux resources
    inotify_rm_watch(fd, wd);
    close(fd);

    printf("Exiting safely.\n");
    return EXIT_SUCCESS;
}
