#include "lexer.h"

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

void parse() {
    Token token = get_next_token();
    while (token.type != T_EOF) {
        switch (token.type) {
            case T_INTEGER:
                printf("Token: type=%d, value=%d\n", token.type, token.value);
                break;
            case T_STRING:
                printf("Token: type=%d, value=%d, string=%s", token.type, token.value, token.string);
                break;
            default:
                printf("Token: type=%d are not being handled in parse()\n", token.type);
                break;
        }
        token = get_next_token();
    }
}

int main(int argc, char* argv[]) {
  printf("Enter an expression: ");
  scanf("%s", text);
  parse();

  return 0;
}
