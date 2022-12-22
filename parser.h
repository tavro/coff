#include <stdio.h>
#include <stdlib.h>

enum AstNodeType {
    AST_NODE,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_BINARYOPERATION,
    AST_BINARYRELATION,
    AST_LVALUE,
    AST_EXPR_LIST,
    AST_STMT_LIST,
    AST_ELSIF_LIST,
    AST_ID,
    AST_INDEXED,
    AST_ADD,
    AST_SUB,
    AST_OR,
    AST_AND,
    AST_MULT,
    AST_DIVIDE,
    AST_IDIV,
    AST_MOD,
    AST_EQUAL,
    AST_NOTEQUAL,
    AST_LESSTHAN,
    AST_GREATERTHAN,
    AST_PROCEDURECALL,
    AST_ASSIGN,
    AST_WHILE,
    AST_IF,
    AST_RETURN,
    AST_FUNCTIONCALL,
    AST_UMINUS,
    AST_NOT,
    AST_ELSIF,
    AST_INTEGER,
    AST_REAL,
    AST_FUNCTIONHEAD,
    AST_PROCEDUREHEAD,
    AST_PARAMETER,
    AST_CAST
};
typedef enum AstNodeType AstNodeType;


typedef struct AstNode {
  AstNodeType type;
  union {
    // AST_INTEGER, AST_REAL
    union {
      int int_val;
      float real_val;
    } value;
  } data;
} AstNode;

enum {
  T_INTNUM,
  T_REALNUM
};

typedef struct {
  int type;
  int row;
  int col;
  int ival;
  float rval;
  char *string;
} Token;

AstNode *parse_factor(Token token);