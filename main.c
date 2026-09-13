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
    // C / C++ family
    {".c",     "//"},
    {".cpp",   "//"},
    {".cxx",   "//"},
    {".cc",    "//"},
    {".h",     "//"},
    {".hpp",   "//"},
    // Modern compiled languages
    {".rs",    "//"},
    {".go",    "//"},
    {".zig",   "//"},
    {".swift", "//"},
    {".kt",    "//"},
    {".kts",   "//"},
    {".scala", "//"},
    {".dart",  "//"},
    // Managed / JVM / .NET
    {".java",  "//"},
    {".cs",    "//"},
    // Web / JS ecosystem
    {".js",    "//"},
    {".mjs",   "//"},
    {".cjs",   "//"},
    {".ts",    "//"},
    {".jsx",   "//"},
    {".tsx",   "//"},
    {".php",   "//"},
    // Hash-comment scripts & configs
    {".py",    "#"},
    {".sh",    "#"},
    {".bash",  "#"},
    {".zsh",   "#"},
    {".rb",    "#"},
    {".r",     "#"},
    {".pl",    "#"},
    {".ps1",   "#"},
    {".yaml",  "#"},
    {".yml",   "#"},
    {".toml",  "#"},
    // Dash-comment languages
    {".lua",   "--"},
    {".sql",   "--"},
    {".hs",    "--"},
    {".ada",   "--"},
    // Windows Batch
    {".bat",   "REM"},
    {".cmd",   "REM"}
};

const int LANG_COUNT = sizeof(SUPPORTED_LANGUAGES) / sizeof(LangMap);

char* strip_bom(char* str) {
    if ((unsigned char)str[0] == 0xEF &&
        (unsigned char)str[1] == 0xBB &&
        (unsigned char)str[2] == 0xBF) {
        return str + 3;
    }
    return str;
}

char* trim_leading_ws(char* str) {
    while (isspace((unsigned char)*str)) str++;
    return str;
}

const char* get_comment_prefix(const char *filename) {
    const char *last_slash = strrchr(filename, '/');
    const char *last_backslash = strrchr(filename, '\\');
    const char *last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;
    const char *dot = strrchr(filename, '.');
    if (!dot || (last_sep && dot <= last_sep)) return "//"; // Default

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

    // Strip UTF-8 BOM if present
    char *cmd = strip_bom(line);

    // Trim leading whitespace before checking prefix
    cmd = trim_leading_ws(cmd);

    // Validate that line begins with expected comment prefix
    size_t prefix_len = strlen(prefix);
    if (strncmp(cmd, prefix, prefix_len) != 0) {
        return NULL;
    }

    cmd += prefix_len;
    cmd = trim_leading_ws(cmd);
    return cmd;
}

int compile_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file '%s'.\n", filename);
        return 1;
    }

    char first_line[4048];
    const char *prefix = get_comment_prefix(filename);

    if (!fgets(first_line, sizeof(first_line), file)) {
        printf("Error: File '%s' is empty.\n", filename);
        fclose(file);
        return 1;
    }
    fclose(file);

    char *cmd = remove_comments(first_line, prefix);
    if (!cmd) {
        printf("Error: First line of '%s' does not start with expected comment prefix '%s'.\n", filename, prefix);
        return 1;
    }
    if (*cmd == '\0') {
        printf("Error: First line of '%s' contains an empty command.\n", filename);
        return 1;
    }

    int exit_code = 0;

    // If file is in a subdirectory, switch to that directory before executing
    const char *last_slash = strrchr(filename, '/');
    const char *last_backslash = strrchr(filename, '\\');
    const char *last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;

    if (last_sep != NULL) {
        char dir[MAX_PATH];
        size_t dir_len = (size_t)(last_sep - filename);
        if (dir_len == 0) {
            dir[0] = *last_sep;
            dir[1] = '\0';
        } else {
            _snprintf(dir, sizeof(dir), "%.*s", (int)dir_len, filename);
        }

        char prev_dir[MAX_PATH];
        DWORD res = GetCurrentDirectoryA(sizeof(prev_dir), prev_dir);
        if (res > 0 && SetCurrentDirectoryA(dir)) {
            exit_code = system(cmd);
            SetCurrentDirectoryA(prev_dir);
        } else {
            exit_code = system(cmd);
        }
    } else {
        exit_code = system(cmd);
    }

    return exit_code;
}

char* check_file(const char *filename) {
    // Supporting both "file" and "file.ext" patterns
    const char *last_slash = strrchr(filename, '/');
    const char *last_backslash = strrchr(filename, '\\');
    const char *last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;
    const char *dot = strrchr(filename, '.');
    bool has_ext = (dot != NULL) && (last_sep == NULL || dot > last_sep);

    char search_pattern[MAX_PATH];
    if (!has_ext) {
        // If no dot in the filename portion, add ".*" to match any extension
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
    FindClose(hFind);

    // If filename contained a directory path, combine it with the matched filename
    if (last_sep != NULL) {
        char full_path[MAX_PATH];
        size_t dir_len = (size_t)(last_sep - filename + 1);
        _snprintf(full_path, sizeof(full_path), "%.*s%s", (int)dir_len, filename, data.cFileName);
        return _strdup(full_path);
    }

    return _strdup(data.cFileName);
}

void help_option() {
    printf("Usage: compile <filename>\n\n");
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
}

void version_option() {
    printf("v1.2.0\n");
}

void handle_options(char *option) {
    if (_stricmp(option, "-h") == 0) help_option();
    else if (_stricmp(option, "-help") == 0) help_option();
    else if (_stricmp(option, "--help") == 0) help_option();
    else if (_stricmp(option, "-v") == 0) version_option();
    else if (_stricmp(option, "--version") == 0) version_option();
    else {
        printf("Error: Unrecognized option: '%s'\n", option);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        help_option();
        return 0;
    } else if (argc > 2) {
        printf("Error: Too many arguments. Please provide only one file.\n");
        return 1;
    }

    if (argv[1][0] == '-') {
        handle_options(argv[1]);
        return 0;
    }

    char *filename = check_file(argv[1]);
    if (!filename) {
        return 1;
    }

    int exit_code = compile_file(filename);
    free(filename);
    return exit_code;
}