#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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

// Token structure
typedef struct {
  int type;      
  int value;     // Token value        (if applicable)
  char *string;  // Token string value (if applicable)
} Token;

// Global variables
int pos = 0;    // Current position in the input text
char text[100]; // Input text

Token get_next_token();
