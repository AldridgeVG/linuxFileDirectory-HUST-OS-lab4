#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void getRWX(const unsigned short m) {
    if (m & S_IRUSR)
        printf("r");
    else
        printf("-");
    if (m & S_IWUSR)
        printf("w");
    else
        printf("-");
    if (m & S_IXUSR)
        printf("x");
    else
        printf("-");

    if (m & S_IRGRP)
        printf("r");
    else
        printf("-");
    if (m & S_IWGRP)
        printf("w");
    else
        printf("-");
    if (m & S_IXGRP)
        printf("x");
    else
        printf("-");

    if (m & S_IROTH)
        printf("r");
    else
        printf("-");
    if (m & S_IWOTH)
        printf("w");
    else
        printf("-");
    if (m & S_IXOTH)
        printf("x");
    else
        printf("-");
}

void printDir(char* dir, int depth) {
    DIR* dp;
    struct dirent* entry;
    struct stat statBuf;

    // fail to open dir
    if ((dp = opendir(dir)) == NULL) {
        printf("dir open failure!\n");
        return;
    }

    // opened dir
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        // set dir name as parameter to call lstat, retrieve dir info
        lstat(entry->d_name, &statBuf);

        if (S_ISDIR(statBuf.st_mode)) {
            if (strcmp(entry->d_name, "..") == 0 ||
                strcmp(entry->d_name, ".") == 0)
                continue;
            // print dir depth,dir name
            //recursively call printdir, depth+=4
            getRWX(statBuf.st_mode);
            printf(" %d  ", depth);

            struct passwd* pwd;
            struct passwd* gpwd;

            // get file owner id
            pwd = getpwuid(statBuf.st_uid);
            printf("%s  ", pwd->pw_name);

            // get file owner group id
            if ((gpwd = getpwuid(statBuf.st_gid)) != NULL)
                printf("%s\t", gpwd->pw_name);

            // get file size
            printf("%ld\t", statBuf.st_size);

            // last modifying time
            printf("%s\t", strtok(ctime(&statBuf.st_mtime) + 4, "\n"));

            printf("%s\n", entry->d_name);
            printDir(entry->d_name, depth + 4);
        } else {
            //printf("%d\t", statBuf.st_mode);
            getRWX(statBuf.st_mode);
            printf(" %d  ", depth);

            struct passwd* pwd;
            struct passwd* gpwd;

            // get file owner id
            pwd = getpwuid(statBuf.st_uid);
            printf("%s  ", pwd->pw_name);

            // get file owner group id
            if ((gpwd = getpwuid(statBuf.st_gid)) != NULL)
                printf("%s\t", gpwd->pw_name);

            // get file size
            printf("%ld\t", statBuf.st_size);

            // last modifying time
            printf("%s\t", strtok(ctime(&statBuf.st_mtime) + 4, "\n"));

            printf("%s\n", entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}

int main() {
    char mdir[100];
    /*
    printf("input directory name:\n");
    scanf("%s", mdir);
    */

    getcwd(mdir, sizeof(mdir));
    printDir(mdir, 1);
    cout << "over" << endl;
    return 0;
}