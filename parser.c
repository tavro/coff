#include "parser.h"

AstNode *parse_factor(Token token) {
  AstNode *node;
  switch (token.type) {
    case T_INTNUM:
      node = malloc(sizeof(AstNode));
      node->type = AST_INTEGER;
      node->data.value.int_val = token.ival;
      break;
    case T_REALNUM:
      node = malloc(sizeof(AstNode));
      node->type = AST_REAL;
      node->data.value.real_val = token.rval;
      break;
    default:
      fprintf(stderr, "Unexpected token '%d'\n", token.type);
      exit(1);
  }
  return node;
}

void print_ast(AstNode *node) {
  switch (node->type) {
    case AST_INTEGER:
      printf("AST_INTEGER: %d\n", node->data.value.int_val);
      break;
    case AST_REAL:
      printf("AST_REAL: %f\n", node->data.value.real_val);
      break;
    default:
      fprintf(stderr, "Unexpected AST type '%d'\n", node->type);
      exit(1);
  }
}

int main(int argc, char **argv) {
  FILE *input_file = fopen("./output/lexer-tokens-out.txt", "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Unable to open input file\n");
    return 1;
  }

  Token tokens[999];
  int num_tokens = 0;

  // Read tokens from file
  char line[256];
  while (fgets(line, sizeof(line), input_file) != NULL) {
    Token token;
    char type_string[12];
    sscanf(line, "Token(%d , %d ): type=%d (%12s) ival=%d rval=%f string=%s", &token.col, &token.row, &token.type, type_string, &token.ival, &token.rval, token.string);
    tokens[num_tokens++] = token;
  }
  fclose(input_file);

  // Print array of tokens
  printf("\n");
  printf("=============================\n");
  printf("Array of Tokens\n");
  printf("=============================\n");
  printf("pos        type        ival    rval        string\n");
  printf("-----------------------------\n");
  for (int i = 0; i < num_tokens; i++) {
    Token token = tokens[i];
    printf("(%-2d, %-2d):  %-12d%-8d%-12f%-12s\n", token.col, token.row, token.type, token.ival, token.rval, token.string);
  }

  for (int i = 0; i < num_tokens; i++) {
    Token token = tokens[i];
    if(token.type == T_INTNUM || token.type == T_REALNUM) {
      AstNode *ast = parse_factor(token);
      print_ast(ast);
    }
  }

  return 0;
}
