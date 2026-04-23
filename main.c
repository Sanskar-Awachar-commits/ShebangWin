// gcc -o compile main.c
// This program compiles many languages by running the commentted code in the first line.
// Warning: The program is prone to security vulnerabilities due to the use of system() and lack of input validation. Use with caution and only in a controlled environment.
// Usage: compile <filename>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *extension;
    const char *comment;
} LangMap;

LangMap SUPPORTED_LANGUAGES[] = {
    {".c",    "//"},
    {".cpp",  "//"},
    {".java", "//"},
    {".py",   "#"},
    {".sh",   "#"},
    {".rb",   "#"},
    {".lua",  "--"},
    {".sql",  "--"},
    {".hs",   "--"},
    {".bat",  "REM"},
    {".ps1",  "#"}
};

const int LANG_COUNT = sizeof(SUPPORTED_LANGUAGES) / sizeof(LangMap);

char* trim_leading_ws(char* str) {
    while(isspace((unsigned char)*str)) str++;
    return str;
}

const char* get_comment_prefix(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return "//"; // Default

    for (int i = 0; i < LANG_COUNT; i++) {
        if (_stricmp(dot, SUPPORTED_LANGUAGES[i].extension) == 0) {
            return SUPPORTED_LANGUAGES[i].comment;
        }
    }
    
    return "//";
}

char *remove_comments(char *line, const char *prefix) {
    // Remove the trailing newline character
    line[strcspn(line, "\r\n")] = '\0';
    // Remove comment characters (//) if present
    char *cmd = line;
    size_t prefix_len = strlen(prefix);
    if (strncmp(line, prefix, prefix_len) == 0) {
        cmd += prefix_len;
    }
    cmd = trim_leading_ws(cmd);
    return cmd;
}

void compile_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file '%s'.\n", filename);
        return;
    }

    char first_line[4048];
    const char *prefix = get_comment_prefix(filename);

    if (fgets(first_line, sizeof(first_line), file)) {
        char *cmd = remove_comments(first_line, prefix);
        system(cmd);
    }

    fclose(file);
}

char* check_file(const char *filename) {
    // Supporting both "file" and "file.ext" patterns
    char search_pattern[MAX_PATH];
    if (strrchr(filename, '.') == NULL) {
        // If no dot add ".*" to match any extension
        _snprintf(search_pattern, sizeof(search_pattern), "%s.*", filename);
    }
    else {
        _snprintf(search_pattern, sizeof(search_pattern), "%s", filename);
    }

    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA(search_pattern, &data);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: File '%s' not found.\n", filename);
        return NULL;
    }
    char* found_name = _strdup(data.cFileName);
    FindClose(hFind);
    return found_name;
}

int main(int argc, char *argv[]) {
    // system("g++ -o main main.cpp");
    // system("./main");
    if (argc == 1) {
        printf("Usage: compile <filename>\n");
    }
    else if (argc > 2) {
        printf("Error: Too many arguments. Please provide only one file.\n");
    }
    else {
        char *filename = check_file(argv[1]);
        if (filename) {
            compile_file(filename);
            free(filename);
        }
    }
    return 0;
}