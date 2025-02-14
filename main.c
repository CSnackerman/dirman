#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_PATH_STACK_SIZE 256
#define MAX_LENGTH 256

int isValidDirectoryChar(char c)
{
    if (c == 45 || c == 95 || c >= 65 && c <= 90 || c >= 97 && c <= 122 || c >= 48 && c <= 57 || c == 40 || c == 41 || c == 32 || c == 126 || c == 46)
    {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please supply a filename.\n");
        return EXIT_SUCCESS;
    }

    FILE *dirmap_file = fopen(argv[1], "r");
    if (dirmap_file == NULL)
    {
        perror("Failed to open file");
        return EXIT_SUCCESS;
    }

    int prev_tab_count = 0;
    char line[512];
    char path_stack[MAX_PATH_STACK_SIZE][MAX_LENGTH] = {"."};
    size_t path_stack_idx = 1;
    while (fgets(line, sizeof(line), dirmap_file))
    {
        char c = '0';
        int tab_count = 0;
        char dirname[128] = "";
        size_t dirname_idx = 0;
        const char TAB_CHAR = 9;
        for (int i = 0; c != '\0' && i < 512; i++)
        {
            c = line[i];

            if (c == TAB_CHAR)
            {
                tab_count++;
            }
            else if (isValidDirectoryChar(c) == 1 && line[i + 1] == '\0')
            {
                dirname[dirname_idx] = c;
                dirname[dirname_idx + 1] = '\0';
            }
            else if (isValidDirectoryChar(c) == 1)
            {
                dirname[dirname_idx] = c;
                dirname_idx++;
            }
        }

        int tab_diff = tab_count - prev_tab_count;
        if (tab_diff < 0)
        {
            // pop diff times
            path_stack_idx += tab_diff;
            strcpy(path_stack[path_stack_idx], dirname);
        }
        else if (tab_diff == 0)
        {
            // replace top of stack
            strcpy(path_stack[path_stack_idx], dirname);
        }
        else
        {
            // push
            path_stack_idx++;
            strcpy(path_stack[path_stack_idx], dirname);
        }

        prev_tab_count = tab_count;

        char path[MAX_LENGTH] = "";
        for (int i = 0; i < path_stack_idx + 1; i++)
        {
            strcat(path, path_stack[i]);
            strcat(path, "/");
        }

        if (mkdir(path, 0777) == 0)
        {
            printf("Created Directory: %s\n", path);
        }
        else
        {
            perror("Failed to create directory: ");
            printf("%s\n", path);

            return EXIT_FAILURE;
        }
    }

    fclose(dirmap_file);

    return EXIT_SUCCESS;
}