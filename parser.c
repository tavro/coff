#include <stdio.h>

typedef enum {
  T_EOF,
  T_PLUS,
  T_MINUS,
  T_MULTIPLY,
  T_DIVIDE,
  T_INTEGER,
  T_ID,
  T_STRING
} TokenType;

typedef struct {
  TokenType type;
  union {
    int integer_value;
    char* string_value;
  };
} Token;

int parse_expression(Token* tokens, int num_tokens) {
  int result = 0;
  int i = 0;
  while (i < num_tokens) {
    if (tokens[i].type == T_INTEGER) {
      result = tokens[i].integer_value;
      i++;
      continue;
    }

    if (tokens[i].type == T_PLUS) {
      i++;
      result += parse_expression(tokens + i, num_tokens - i);
      break;
    } else if (tokens[i].type == T_MINUS) {
      i++;
      result -= parse_expression(tokens + i, num_tokens - i);
      break;
    }

    if (tokens[i].type == T_MULTIPLY) {
      i++;
      result *= parse_expression(tokens + i, num_tokens - i);
    } else if (tokens[i].type == T_DIVIDE) {
      i++;
      result /= parse_expression(tokens + i, num_tokens - i);
    }
  }

  return result;
}

int main(int argc, char* argv[]) {
  Token tokens[] = {
    { T_INTEGER, .integer_value = 1 },
    { T_PLUS },
    { T_INTEGER, .integer_value = 2 },
    { T_MINUS },
    { T_INTEGER, .integer_value = 3 }
  };
  int num_tokens = sizeof(tokens) / sizeof(Token);

  int result = parse_expression(tokens, num_tokens);
  printf("Result: %d\n", result);

  return 0;
}
