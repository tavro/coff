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
  T_INTNUM,
  T_REALNUM
};

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

#define RESERVED_WORD_COUNT 12
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
  {"then", T_THEN},
  {"else", T_ELSE},
  {"const", T_CONST},
  {"array", T_ARRAY},
  {"begin", T_BEGIN}
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
int pos = 0;
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