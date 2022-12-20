#include <stdio.h>
#include <ctype.h>

// Enumeration of token types
enum {
  T_EOF,       // End of file
  T_PLUS,      
  T_MINUS,     
  T_MULTIPLY,  
  T_DIVIDE,    
  T_INTEGER,   
};

// Token structure
typedef struct {
  int type;      
  int value;     // Token value (if applicable)
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
    printf("Token: type=%d, value=%d\n", token.type, token.value);
    token = get_next_token();
  }

  return 0;
}
