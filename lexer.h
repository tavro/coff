#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 1000
#define MAX_TOKENS  1000

// Enumeration of token types
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
  T_INTNUM,
  T_REALNUM
};

typedef struct {
  int type;      
  int value;     // Token value        (if applicable)
  char *string;  // Token string value (if applicable)
} Token;

typedef struct {
  char *name;
  int type;
  int value;
} Symbol;

// Global variables
int pos = 0;    // Current position in the input text
char text[100]; // Input text

Token tokens[MAX_TOKENS];
int num_tokens = 0;

Symbol symbol_table[MAX_SYMBOLS];
int num_symbols = 0;

Token get_next_token();
Token* get_token(int index);
void add_token(int type, int value, char* str);

Symbol* lookup_symbol(char *name);

void add_symbol(char *name, int type, int value);

void print_symbol_table(Symbol *symbol_table, int num_symbols);