#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define INDENT_STEP 2

static void print_indent(int level);
static void tree(const char *path, int level);


static void print_indent(int level) {
    for (int i = 0; i < level * INDENT_STEP; i++) {
        printf(" ");
    }
}

static void tree(const char *path, int level) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char fullpath[1024];

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "tree: no se puede abrir '%s': %s\n", path, 
strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || 
strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, 
entry->d_name);

        if (lstat(fullpath, &st) == -1) {
            fprintf(stderr, "tree: error con '%s': %s\n", fullpath, 
strerror(errno));
            continue;
        }

        print_indent(level);
        printf("%s\n", entry->d_name);

        if (S_ISDIR(st.st_mode)) {
            tree(fullpath, level + 1);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path;

    if (argc > 2) {
        fprintf(stderr, "Uso: tree [ruta]\n");
        return 1;
    }

    if (argc == 1) {
        path = ".";
    } else {
        path = argv[1];
    }

    printf("%s\n", path);
    tree(path, 1);

    return 0;
}
