#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Enumeration of token types
enum {
  T_EOF,       // End of file
  T_PLUS,      
  T_MINUS,     
  T_MULTIPLY,  
  T_DIVIDE,    
  T_INTEGER,   
  T_ID,        // Identifier
  T_STRING,    
};

// Token structure
typedef struct {
  int type;      
  int value;     // Token value        (if applicable)
  char *string;  // Token string value (if applicable)
} Token;

// Global variables
int pos = 0;    // Current position in the input text
char text[100]; // Input text
FILE *input_file;

void open_input_file(char *filename) {
  input_file = fopen(filename, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: unable to open input file\n");
    exit(1);
  }
}

void close_input_file() {
  fclose(input_file);
}

// Reads the next character from the input file
char get_next_char() {
  int c = fgetc(input_file);
  if (c == EOF) {
    return '\0';
  }
  else {
    return (char)c;
  }
}

Token get_next_token() {
  Token token;

  char c = get_next_char();

  // Skip any leading whitespace characters
  while (isspace(c)) {
    c = get_next_char();
  }
  
  // Check for end of input
  if (c == '\0') {
    token.type = T_EOF;
  }
  // Check for integer value
  else if (isdigit(c)) {
    token.type = T_INTEGER;
    token.value = 0;
    while (isdigit(c)) {
      // Compute integer value by adding the digit to the existing value multiplied by 10
      token.value = token.value * 10 + (c - '0');
      c = get_next_char();
    }
  }
  else if (c == '+') {
    token.type = T_PLUS;
    c = get_next_char();
  }
  else if (c == '-') {
    token.type = T_MINUS;
    c = get_next_char();
  }
  else if (c == '*') {
    token.type = T_MULTIPLY;
    c = get_next_char();
  }
  else if (c == '/') {
    token.type = T_DIVIDE;
    c = get_next_char();
  }
  // Check for identifier
  else if (isalpha(c)) {
    token.type = T_ID;
    int start = pos;
    pos++;
    while (isalnum(c)) {
      c = get_next_char();
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, text + start, length);
    token.string[length] = '\0';
  }
  // Check for string value
  else if (c == '"') {
    token.type = T_STRING;
    int start = pos + 1;
    c = get_next_char();
    while (c != '"') {
      c = get_next_char();
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, text + start, length);
    token.string[length] = '\0';
    c = get_next_char();
  }
  // Otherwise, return an error
  else {
    fprintf(stderr, "Error: invalid character '%c'\n", c);
    exit(1);
  }

  return token;
}

int main(int argc, char *argv[]) {
  // Check for the correct number of arguments
  if (argc != 2) {
    fprintf(stderr, "Error: invalid number of arguments\n");
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  open_input_file(argv[1]);

  Token token;
  do {
    token = get_next_token();

    printf("Token: type=%d, value=%d", token.type, token.value);
    if (token.type == T_ID) {
      printf(", string=%s", token.string);
    }
    else if (token.type == T_STRING) {
      printf(", string=%s", token.string);
    }
    printf("\n");
  } while (token.type != T_EOF);
  close_input_file();

  return 0;
}
