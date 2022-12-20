#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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

Token get_next_token() {
  Token token;

  // Skip any leading whitespace characters
  while (isspace(text[pos])) {
    pos++;
  }

  // Check for end of input
  if (text[pos] == '\0') {
    token.type = T_EOF;
  }
  else if (isdigit(text[pos])) {
    token.type = T_INTEGER;
    token.value = 0;
    while (isdigit(text[pos])) {
      // Compute integer value by adding the digit to the existing value multiplied by 10
      token.value = token.value * 10 + (text[pos] - '0');
      pos++;
    }
  }
  else if (text[pos] == '+') {
    token.type = T_PLUS;
    pos++;
  }
  else if (text[pos] == '-') {
    token.type = T_MINUS;
    pos++;
  }
  else if (text[pos] == '*') {
    token.type = T_MULTIPLY;
    pos++;
  }
  else if (text[pos] == '/') {
    token.type = T_DIVIDE;
    pos++;
  }
  // Check for identifier
  else if (isalpha(text[pos])) {
    token.type = T_ID;
    int start = pos;
    pos++;
    while (isalnum(text[pos])) {
      pos++;
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, text + start, length);
    token.string[length] = '\0';
  }
  // Check for string value
  else if (text[pos] == '"') {
    token.type = T_STRING;
    int start = pos + 1;
    pos++;
    while (text[pos] != '"') {
      pos++;
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, text + start, length);
    token.string[length] = '\0';
    pos++;
  }
  // Otherwise, invalid character
  else {
    printf("Error: invalid character '%c'\n", text[pos]);
  }

  return token;
}

int main() {
  // Read input text from the user
  printf("Enter an expression: ");
  scanf("%s", text);

  Token token = get_next_token();

  // Continue until the end of the input is reached
  while (token.type != T_EOF) {
    printf("Token: type=%d, value=%d", token.type, token.value);
    if (token.type == T_STRING || token.type == T_ID) {
      printf(", string=%s", token.string);
    }
    printf("\n");
    token = get_next_token();
  }

  return 0;
}