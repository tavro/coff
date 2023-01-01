/*
 * Copyright (c) 2022 Isak Horvath <isakhorvath@gmail.com>
 *
 * The Coff Compiler is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software Foundation
 *
 */

#ifndef COFF_LEXER_H
#define COFF_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SYMBOLS 1000
#define MAX_TOKENS  1000

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
  T_ARG,
  T_IDIV,
  T_MOD,
  T_NOT,
  T_THEN,
  T_ELSE,
  T_TRUE,
  T_CONST,
  T_ARRAY,
  T_BEGIN,
  T_WHILE,
  T_ELSIF,
  T_PRINT,
  T_FALSE,
  T_RETURN,
  T_ID,
  T_PROGRAM,
  T_PROCEDURE,
  T_FUNCTION,
  T_INTTYPE,
  T_REALTYPE,
  T_BOOLTYPE,
  T_STRTYPE,
  T_STR,
  T_INTNUM,
  T_REALNUM
};

enum {
  S_INT,
  S_REAL,
  S_STR,
  S_BOOL,
  S_VOID,
  S_NONE
};

const char* symbol_to_string(int symbol) {
  switch (symbol) {
    case S_INT: return "S_INT";
    case S_REAL: return "S_REAL";
    case S_STR: return "S_STR";
    case S_BOOL: return "S_BOOL";
    case S_VOID: return "S_VOID";
    case S_NONE: return "S_NONE";
    default: return "INVALID TOKEN";
  }
}

const char* token_to_string(int token) {
  switch (token) {
    case T_EOF: return "T_EOF";
    case T_ERROR: return "T_ERROR";
    case T_EQ: return "T_EQ";
    case T_DOT: return "T_DOT";
    case T_SEMICOLON: return "T_SEMICOLON";
    case T_COLON: return "T_COLON";
    case T_LEFTBRACKET: return "T_LEFTBRACKET";
    case T_RIGHTBRACKET: return "T_RIGHTBRACKET";
    case T_LEFTPAR: return "T_LEFTPAR";
    case T_RIGHTPAR: return "T_RIGHTPAR";
    case T_COMMA: return "T_COMMA";
    case T_LESSTHAN: return "T_LESSTHAN";
    case T_GREATERTHAN: return "T_GREATERTHAN";
    case T_ADD: return "T_ADD";
    case T_SUB: return "T_SUB";
    case T_MUL: return "T_MUL";
    case T_RDIV: return "T_RDIV";
    case T_IF: return "T_IF";
    case T_DO: return "T_DO";
    case T_ASSIGN: return "T_ASSIGN";
    case T_NOTEQ: return "T_NOTEQ";
    case T_OR: return "T_OR";
    case T_VAR: return "T_VAR";
    case T_END: return "T_END";
    case T_AND: return "T_AND";
    case T_ARG: return "T_ARG";
    case T_IDIV: return "T_IDIV";
    case T_MOD: return "T_MOD";
    case T_NOT: return "T_NOT";
    case T_THEN: return "T_THEN";
    case T_ELSE: return "T_ELSE";
    case T_CONST: return "T_CONST";
    case T_ARRAY: return "T_ARRAY";
    case T_BEGIN: return "T_BEGIN";
    case T_WHILE: return "T_WHILE";
    case T_ELSIF: return "T_ELSIF";
    case T_PRINT: return "T_PRINT";
    case T_RETURN: return "T_RETURN";
    case T_ID: return "T_ID";
    case T_PROGRAM: return "T_PROGRAM";
    case T_PROCEDURE: return "T_PROCEDURE";
    case T_FUNCTION: return "T_FUNCTION";
    case T_INTTYPE: return "T_INTTYPE";
    case T_REALTYPE: return "T_REALTYPE";
    case T_BOOLTYPE: return "T_BOOLTYPE";
    case T_STRTYPE: return "T_STRTYPE";
    case T_INTNUM: return "T_INTNUM";
    case T_REALNUM: return "T_REALNUM";
    default: return "INVALID TOKEN";
  }
}

#define TOKEN_COUNT 16
struct TokenMap {
  char c;
  int type;
} token_map[TOKEN_COUNT] = {
  {'=', T_ASSIGN},
  {'.', T_DOT},
  {';', T_SEMICOLON},
  {':', T_COLON},
  {'[', T_LEFTBRACKET},
  {']', T_RIGHTBRACKET},
  {'(', T_LEFTPAR},
  {')', T_RIGHTPAR},
  {',', T_COMMA},
  {'<', T_LESSTHAN},
  {'>', T_GREATERTHAN},
  {'+', T_ADD},
  {'-', T_SUB},
  {'*', T_MUL},
  {'/', T_RDIV},
  {'\0', T_EOF}
};

#define RESERVED_WORD_COUNT 23
struct ReservedWord {
  char *word;
  int type;
} reserved_words[RESERVED_WORD_COUNT] = {
  {"if", T_IF},
  {"do", T_DO},
  {"or", T_OR},
  {"var", T_VAR},
  {"end", T_END},
  {"and", T_AND},
  {"not", T_NOT},
  {"arg", T_ARG},
  {"int", T_INTTYPE},
  {"real", T_REALTYPE},
  {"bool", T_BOOLTYPE},
  {"str", T_STRTYPE},
  {"then", T_THEN},
  {"else", T_ELSE},
  {"func", T_FUNCTION},
  {"true", T_TRUE},
  {"false", T_FALSE},
  {"const", T_CONST},
  {"array", T_ARRAY},
  {"begin", T_BEGIN},
  {"print", T_PRINT},
  {"return", T_RETURN},
  {"program", T_PROGRAM}
};

typedef struct {
  int type;
  int row;       // Line number in source file
  int col;       // Column number in source file
  int ival;     // Token int value    (if applicable)
  float rval;     // Token float value  (if applicable)
  char *string;  // Token string value (if applicable)
} Token;

typedef struct {
  char *name;
  int sym_type;
  int tok_type;
  int value;
  float rval;
  char* str_val;
} Symbol;

// Global variables
int pos = 0;
int line = 0;
int column = 0;
char text[100];

Token tokens[MAX_TOKENS];
int num_tokens = 0;

Symbol symbol_table[MAX_SYMBOLS];
int num_symbols = 0;

#define TOKEN_STRING_BUFFER_SIZE 128
char token_string_buffer[TOKEN_STRING_BUFFER_SIZE];

// Function declarations
Token get_next_token();
Token* get_token(int index);

Symbol* lookup_symbol(char *name) {
  for (int i = 0; i < num_symbols; i++) {
    if (strcmp(symbol_table[i].name, name) == 0) {
      return &symbol_table[i];
    }
  }
  return NULL;
}

void add_token(int type, int value, char* str);

void add_symbol(char *name, int type, int value) {
  if(!lookup_symbol(name)) {
    symbol_table[num_symbols].name = strdup(name);
    symbol_table[num_symbols].tok_type = type;
    symbol_table[num_symbols].sym_type = S_NONE;
    symbol_table[num_symbols].value = value;
    num_symbols++;
  }
}

void write_symbol_table(const char *filename, Symbol *symbol_table, int num_symbols) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening file");
    return;
  }

  fprintf(fp, "=============================================\n");
  fprintf(fp, "Symbol Table\n");
  fprintf(fp, "=============================================\n");
  fprintf(fp, "name        t_type        s_type        value\n");
  fprintf(fp, "---------------------------------------------\n");
  for (int i = 0; i < num_symbols; i++) {
    Symbol symbol = symbol_table[i];
    fprintf(fp, "%-12s%-2d=%-11s%-1d=%-12s%d\n", symbol.name, symbol.tok_type, token_to_string(symbol.tok_type), symbol.sym_type, symbol_to_string(symbol.sym_type), symbol.value);
  }

  fclose(fp);
}

void print_symbol_table(Symbol *symbol_table, int num_symbols) {
  printf("\n");
  printf("=============================================\n");
  printf("Symbol Table\n");
  printf("=============================================\n");
  printf("name        t_type        s_type        value\n");
  printf("---------------------------------------------\n");

  for (int i = 0; i < num_symbols; i++) {
    Symbol symbol = symbol_table[i];
    printf("%-12s%-2d=%-11s%-1d=%-12s%d\n", symbol.name, symbol.tok_type, token_to_string(symbol.tok_type), symbol.sym_type, symbol_to_string(symbol.sym_type), symbol.value);
  }
}

#endif