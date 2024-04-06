#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

חלק1
void welcome() {
    printf("aSPY//YASa\n");
    printf("apyyyyCY//////////YCa\n");
    printf("Welcome to myShell\n");
    printf("SY//////YSpcs\n");
    printf("scpCY//Pp\n");
    printf("ayp ayyyyyyySCP//PP\n");
    printf("Version 2.4.3\n");
    printf("syY//C\n");
    printf("CY//S\n");
    printf("AYASAYYYYYYYY///PS\n");
    printf("y//Y\n");
    printf("https://github.com/Mariamdrawshy\n");
    printf("cS5ps\n");
    printf("PCCCCY//p\n");
    printf("PP///AC//Y\n");
    printf("SPPPP///a\n");
    printf("Have fun!\n");
    printf("суP////C |\n");
    printf("A//A\n");
    printf("p///Ac\n");
    printf("SC///a\n");
    printf("P////YCpc\n");
    printf("A//A\n");
    printf("Craft packets Like it is your Last\n");
    printf("day on earth.\n");
    printf("scccccp///PSP///p\n");
    printf("p//Y\n");
    printf("caa\n");
    printf("SY/////////y\n");
    printf("S//P\n");
    printf("Lao-Tze\n");
    printf("cayCyayP//Ya\n");
    printf("pY/Ya\n");
    printf("aC//Yp\n");
    printf("sY/PsY////YCC\n");
    printf("sc sccaCY//PCypaapyCP//YSs\n");
    printf("spCPY//////YPSps\n");
    printf("ccaacs\n");
    printf("using\n");
}

void getLocation() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\033[1;32m%s\033[0m\n", cwd);
        // אם הצליח לקרוא את נתיב הנוכחי של התיקייה
    } else {
        perror("getcwd() error");
        // אם נתיב הנוכחי לא נקרא בהצלחה, מדפיס שגיאה
    }
}

char **splitArgument(char *str) {
    char **args = malloc(strlen(str) * sizeof(char *));
    // מקצה זיכרון עבור המחרוזת שנקלטה בפונקציה
    if (args == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
         // אם המקצוע של זיכרון נכשל, מדפיס הודעת שגיאה ויוצא מהתוכנית
        exit(1);
    }

    char *token = strtok(str, " ");
     // מפצל את המחרוזת למחרוזות קטנות לפי רווח
    int i = 0;
    while (token != NULL) {
        args[i] = malloc((strlen(token) + 1) * sizeof(char));
         // מקצה זיכרון עבור כל מחרוזת קטנה שנוצרה

        if (args[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            // אם המקצוע של זיכרון נכשל, מדפיס הודעת שגיאה ויוצא מהתוכנית
            exit(1);
        }
        strcpy(args[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
     // מסמן את המקום האחרון במערך ל-NULL, כדי לסיים את המערך

    return args;
      // מחזיר את המערך של המחרוזות הקטנות
}

חלק2
/* 
    * Logout
    * args: array of strings
    * each arg is a command line argument
    * returns: void
*/
void logout(char *str) {
    if(str == NULL) {
        exit(0);
    }
    printf("%s\n", str);
    exit(0);
}

/* 
    * Build Path
    * args: array of strings
    * each arg is path components
    * each path component is separated by a space
*/

char *build_path(char **args, int num_args) {
    char *path = malloc(100 * sizeof(char));
    if (path == NULL) {
        fprintf(stderr, "Failed to allocate memory for path.\n");
        exit(1);
    }

    // Start with an empty string
    path[0] = '\0';

    for (int i = 0; i < num_args; i++) {
        // If the argument starts with a quote, find the closing quote
        if (args[i][0] == '"') {
            char *end_quote = strchr(args[i] + 1, '"');
            if (end_quote) {
                // Remove the quotes
                args[i]++;
                *end_quote = '\0';
            } else {
                fprintf(stderr, "Missing closing quote in argument.\n");
                exit(1);
            }
        }

        strcat(path, args[i]);

        // Only append "/" if it's not the last argument and the argument doesn't end with a quote
        if (i < num_args - 1 && args[i][strlen(args[i]) - 1] != '"') {
            strcat(path, "/");
        }
    }
    return path;
}


/* 
    * Change Directory
    * args: array of strings
    * each arg is a command line argument
    * returns: void
*/
void cd(char **args) {
    // Count the number of arguments
    int num_args = 0; 
    while (args[num_args] != NULL) {
        num_args++;
    }

    if (num_args < 1) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    char *path = build_path(args, num_args);
    printf("Changing directory to: %s\n", path);
    if(chdir(path) != 0) {
        perror("cd");
    }
}


/* 
    * Copy File
    * args: array of strings
    * each arg is a command line argument
    * returns: void
*/

void copy_file(char **args) {
    // Count the number of arguments
    int num_args = 0; 
    while (args[num_args] != NULL) {
        num_args++;
    }

    if (num_args < 2) {
        fprintf(stderr, "copy_file: missing argument\n");
        return;
    }

    char *source_path = build_path(&args[0], 1);
    char *destination_path = build_path(&args[1], 1);

    // Check if destination is a directory
    struct stat s;
    if (stat(destination_path, &s) == 0 && S_ISDIR(s.st_mode)) {
        // If destination is a directory, append the source filename to the destination path
        char *source_filename = strrchr(source_path, '/');
        if (source_filename == NULL) {
            source_filename = source_path;
        } else {
            source_filename++;
        }

        char *new_destination_path = malloc(strlen(destination_path) + strlen(source_filename) + 2);
        strcpy(new_destination_path, destination_path);
        strcat(new_destination_path, "/");
        strcat(new_destination_path, source_filename);

        free(destination_path);
        destination_path = new_destination_path;
    }

    FILE *source, *destination;
    char ch;

    // Open source file for reading
    source = fopen(source_path, "r");
    if (source == NULL) {
        fprintf(stderr, "Unable to open source file.\n");
        return;
    }

    // Open destination file for writing
    destination = fopen(destination_path, "w");
    if (destination == NULL) {
        fclose(source);
        fprintf(stderr, "Unable to open destination file.\n");
        return;
    }

    // Copy file
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, destination);
    }

    printf("File copied successfully.\n");

    // Close files
    fclose(source);
    fclose(destination);
}

חלק4
void move_file(char **args) {
    // Count the number of arguments
    int num_args = 0; 
    while (args[num_args] != NULL) {
        num_args++;
    }

    if (num_args < 2) {
        fprintf(stderr, "move_file: missing argument\n");
        return;
    }

    char *source_path = build_path(&args[0], 1);
    printf("source_path: %s\n", source_path);  // Debug print

    char *destination_path = build_path(&args[1], 1);
    printf("destination_path: %s\n", destination_path);  // Debug print

    // Check if destination is a directory
    struct stat s;
    if (stat(destination_path, &s) == 0 && S_ISDIR(s.st_mode)) {
        // If destination is a directory, append the source filename to the destination path
        char *source_filename = strrchr(source_path, '/');
        if (source_filename == NULL) {
            source_filename = source_path;
        } else {
            source_filename++;
        }
        printf("source_filename: %s\n", source_filename);  // Debug print

        char *new_destination_path = malloc(strlen(destination_path) + strlen(source_filename) + 2);
        if (new_destination_path == NULL) {
            fprintf(stderr, "malloc failed\n");
            return;
        }
        strcpy(new_destination_path, destination_path);
        strcat(new_destination_path, "/");
        strcat(new_destination_path, source_filename);

        free(destination_path);
        destination_path = new_destination_path;
    }

    // Move file
    if (rename(source_path, destination_path) != 0) {
        perror("Unable to move file");
        return;
    }

    printf("File moved successfully.\n");
}


/*
    * echo
    * args: array of strings
*/

void echo(char **args) {
    char *str = malloc(1024);  // Allocate initial memory
    if (str == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    str[0] = '\0';  // Initialize str to an empty string

    int i = 0;
    char *mode = "w";  // Default to overwrite mode

    while(args[i] != NULL){
        printf("args[%d]: %s\n", i, args[i]);  // Debug print
        if (strcmp(args[i], ">") == 0) {
            break;
        } else {
            // Check if str has enough space for the next argument
            if (strlen(str) + strlen(args[i]) + 1 > 1024) {
                // Resize str
                str = realloc(str, strlen(str) + strlen(args[i]) + 1);
                if (str == NULL) {
                    printf("Memory allocation failed\n");
                    return;
                }
            }
            strcat(str, args[i]);
            strcat(str, " ");  // Add space between words
        }
        i++;
    }

    if (args[i] == NULL) {
        printf("Missing file path\n");
        free(str);  // Free memory
        return;
    }

    char *path = build_path(&args[i+1], 1);
    printf("path: %s\n", path);  // Debug print

    FILE *fptr;
    fptr = fopen(path, mode);
    if(fptr == NULL)
    {
        printf("Error opening file!\n");   
        free(str);  // Free memory
        return;             
    }
    fprintf(fptr,"%s",str);
    fclose(fptr);
    printf("File updated successfully.\n");

    free(str);  // Free memory
}

void read_file(char **args) {
    char *path = build_path(args, 1);

    FILE *fptr;
    fptr = fopen(path, "r");
    if(fptr == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    char c;
    while((c = fgetc(fptr)) != EOF)
    {
        printf("%c", c);
    }
    fclose(fptr);
    printf("\n");
}

void word_line_count(char **args) {
    FILE *fptr;
    char *path = build_path(&args[1], 1);
    char ch;
    int lines = 1, words = 1, characters = 1;

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    while((ch = fgetc(fptr)) != EOF) {
        characters++;
        if (ch == '\n' || ch == '\0') {
            lines++;
        }
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0') {
            words++;
        }
    }

    if (strcmp(args[0], "-l") == 0) {
        printf("Lines : %d\n", lines);
    }
    else if (strcmp(args[0], "-w") == 0) {
        printf("Words : %d\n", words);
    }
    else {
        printf("Invalid option\n");
    }

    fclose(fptr);
}

/* 
    * Tokenize
    * args: string
    * returns: array of strings
*/
char **tokenize(char *line) {
    char **tokens = malloc(100 * sizeof(char*));
    char *token;
    int i = 0;

    while (*line != '\0') {
        while (isspace(*line)) {
            ++line;  // Skip spaces
        }

        if (*line == '"') {
            ++line;  // Skip the opening quote
            token = line;
            while (*line != '\0' && *line != '"') {
                ++line;
            }
        } else {
            token = line;
            while (*line != '\0' && !isspace(*line)) {
                ++line;
            }
        }

        if (*line != '\0') {
            *line = '\0';
            ++line;
        }

        tokens[i] = token;
        ++i;
    }

    tokens[i] = NULL;  // Set the last element to NULL
    return tokens;
}


int main() {
    char *line = NULL;

    do {
        // Get line
        size_t len = 0;
        getline(&line, &len, stdin);

        // Remove the newline character
        line[strcspn(line, "\n")] = 0;

        // Split the line into tokens
        char **tokens = tokenize(line);

        // The first token is the command
        char *cmd = tokens[0];

        // The rest of the tokens are the arguments
        char **args = &tokens[1];

        if(strcmp(cmd, "exit") == 0) {
            logout(args[0]);
        }
        else if(strcmp(cmd, "cd") == 0) {
            cd(args);
        }
        else if(strcmp(cmd, "cp") == 0) {
            copy_file(args);
        }
        else if(strcmp(cmd, "mv") == 0) {
            move_file(args);
        }
        else if(strcmp(cmd, "echo") == 0) {
            echo(args);
        }
        else if(strcmp(cmd, "read") == 0) {
            read_file(args);
        }
        else if(strcmp(cmd, "wc") == 0) {
            word_line_count(args);
        }

        else {
            printf("Invalid command\n");
        }

        free(tokens);

    } while (line != NULL);
    
    return 0;
}
