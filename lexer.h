#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Enumeration of token types
enum {
  T_EOF,       // End of file
  T_PLUS,      
  T_MINUS,     
  T_MULTIPLY,  
  T_DIVIDE,    
  T_INTEGER,   
  T_ID,        // Identifier
  T_STRING,    
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