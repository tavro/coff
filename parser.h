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
  T_EOF,
  T_ERROR,
  T_EQ,
  T_DOT,
  T_SEMICOLON,
  T_COLON,
  T_LEFTBRACKET,
  T_RIGHTBRACKET,
  T_LEFTPAR,
  T_RIGHTPAR,
  T_COMMA,
  T_LESSTHAN,
  T_GREATERTHAN,
  T_ADD,
  T_SUB,
  T_MUL,
  T_RDIV,
  T_IF,
  T_DO,
  T_ASSIGN,
  T_NOTEQ,
  T_OR,
  T_VAR,
  T_END,
  T_AND,
  T_IDIV,
  T_MOD,
  T_NOT,
  T_THEN,
  T_ELSE,
  T_CONST,
  T_ARRAY,
  T_BEGIN,
  T_WHILE,
  T_ELSIF,
  T_RETURN,
  T_ID,
  T_PROGRAM,
  T_PROCEDURE,
  T_FUNCTION,
  T_INTTYPE,
  T_REALTYPE,
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