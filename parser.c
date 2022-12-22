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
  Token int_token;
  int_token.type = T_INTNUM;
  int_token.row = 1;
  int_token.col = 4;
  int_token.ival = 42;
  int_token.string = NULL;

  Token real_token;
  real_token.type = T_REALNUM;
  real_token.row = 2;
  real_token.col = 2;
  real_token.rval = 42.123;
  real_token.string = NULL;

  AstNode *ast1 = parse_factor(int_token);
  AstNode *ast2 = parse_factor(real_token);
  print_ast(ast1);
  print_ast(ast2);
  return 0;
}
