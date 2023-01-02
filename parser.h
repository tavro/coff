/*
 * Copyright (c) 2022 Isak Horvath <isakhorvath@gmail.com>
 *
 * The Coff Compiler is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software Foundation
 *
 */

#ifndef COFF_PARSER_H
#define COFF_PARSER_H

#include "lexer.h"

enum AstNodeType {
    AST_PROGRAM,
    AST_VARDEC,
    AST_ARG,
    AST_FUNCDEC,
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
    AST_FUNCCALL,
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
    AST_UMINUS,
    AST_NOT,
    AST_ELSIF,
    AST_STR,
    AST_INT,
    AST_REAL,
    AST_BOOL,
    AST_PRINT,
    AST_ARGLIST,
    AST_PARAMLIST,
    AST_PARAM,
    AST_CAST,
    AST_TYPE
};
typedef enum AstNodeType AstNodeType;

typedef struct AstNode {
  AstNodeType type;

  char* val_type;
  union {
    // AST_INT, AST_REAL
    union {
      int int_val;
      float real_val;
      char* char_val;
    } value;
  } data;
  struct AstNode* left;
  struct AstNode* right;

  struct AstNode** children;
  int num_children;
} AstNode;

#endif