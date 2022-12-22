#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
    case T_RETURN: return "T_RETURN";
    case T_ID: return "T_ID";
    case T_PROGRAM: return "T_PROGRAM";
    case T_PROCEDURE: return "T_PROCEDURE";
    case T_FUNCTION: return "T_FUNCTION";
    case T_INTTYPE: return "T_INTTYPE";
    case T_REALTYPE: return "T_REALTYPE";
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

#define RESERVED_WORD_COUNT 16
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
  {"int", T_INTTYPE},
  {"real", T_REALTYPE},
  {"then", T_THEN},
  {"else", T_ELSE},
  {"const", T_CONST},
  {"array", T_ARRAY},
  {"begin", T_BEGIN},
  {"program", T_PROGRAM},
  {"function", T_FUNCTION}
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
  int type;
  int value;
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

Symbol* lookup_symbol(char *name);

void add_token(int type, int value, char* str);
void add_symbol(char *name, int type, int value);
void print_symbol_table(Symbol *symbol_table, int num_symbols);