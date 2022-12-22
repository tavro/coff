#include "lexer.h"

typedef enum {
  AST_PROGRAM,
  AST_VARIABLE,
  AST_FUNCTION,
  AST_BLOCK,
  AST_RETURN,
  AST_ID,
  AST_INT
} AstNodeType;

typedef struct AstNode {
  AstNodeType type;
  int intval;
  char* strval;
  struct AstNode* left;
  struct AstNode* right;
} AstNode;

Token tokenseq[20] = {
    { T_PROGRAM, 0, NULL },
    { T_ID, 0, "nameid" },
    { T_COLON, 0, NULL },
    { T_VAR, 0, NULL },
    { T_ID, 0, "id" },
    { T_COLON, 0, NULL },
    { T_INT, 0, NULL },
    { T_SEMICOLON, 0, NULL },
    { T_FUNCTION, 0, NULL },
    { T_ID, 0, "test" },
    { T_LEFTPAR, 0, NULL },
    { T_ID, 0, "arg" },
    { T_COLON, 0, NULL },
    { T_INT, 0, NULL },
    { T_RIGHTPAR, 0, NULL },
    { T_COLON, 0, NULL },
    { T_INT, 0, NULL},
    { T_COLON, 0, NULL },
    { T_SEMICOLON, 0, NULL },
    { T_SEMICOLON, 0, NULL }
};

int cur_token = 0;