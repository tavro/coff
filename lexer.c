#include "lexer.h"

Token get_next_token() {
  Token token;
  token.string = NULL;
  token.value = 0;

  // Skip whitespace
  while (isspace(text[pos])) {
    pos++;
  }

  // Check for end of file
  if (text[pos] == '\0') {
    token.type = T_EOF;
    return token;
  }

  // Check for single-character tokens
  switch (text[pos]) {
    case '=':
      token.type = T_ASSIGN;
      pos++;
      return token;
    case '.':
      token.type = T_DOT;
      pos++;
      return token;
    case ';':
      token.type = T_SEMICOLON;
      pos++;
      return token;
    case ':':
      token.type = T_COLON;
      pos++;
      return token;
    case '[':
      token.type = T_LEFTBRACKET;
      pos++;
      return token;
    case ']':
      token.type = T_RIGHTBRACKET;
      pos++;
      return token;
    case '(':
      token.type = T_LEFTPAR;
      pos++;
      return token;
    case ')':
      token.type = T_RIGHTPAR;
      pos++;
      return token;
    case ',':
      token.type = T_COMMA;
      pos++;
      return token;
    case '<':
      token.type = T_LESSTHAN;
      pos++;
      return token;
    case '>':
      token.type = T_GREATERTHAN;
      pos++;
      return token;
    case '+':
      token.type = T_ADD;
      pos++;
      return token;
    case '-':
      token.type = T_SUB;
      pos++;
      return token;
    case '*':
      token.type = T_MUL;
      pos++;
      return token;
    case '/':
      token.type = T_RDIV;
      pos++;
      return token;
  }

  // Check for reserved words and identifiers
  if (isalpha(text[pos])) {
    int start = pos;
    while (isalnum(text[pos])) {
      pos++;
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, &text[start], length);
    token.string[length] = '\0';

    if (strcmp(token.string, "if") == 0) {
      token.type = T_IF;
    } else if (strcmp(token.string, "do") == 0) {
      token.type = T_DO;
    } else if (strcmp(token.string, "or") == 0) {
      token.type = T_OR;
    } else if (strcmp(token.string, "var") == 0) {
      token.type = T_VAR;
    } else if (strcmp(token.string, "end") == 0) {
      token.type = T_END;
    } else if (strcmp(token.string, "and") == 0) {
      token.type = T_AND;
    } else if (strcmp(token.string, "not") == 0) {
      token.type = T_NOT;
    } else if (strcmp(token.string, "then") == 0) {
      token.type = T_THEN;
    } else if (strcmp(token.string, "else") == 0) {
      token.type = T_ELSE;
    } else if (strcmp(token.string, "const") == 0) {
      token.type = T_CONST;
    } else if (strcmp(token.string, "array") == 0) {
      token.type = T_ARRAY;
    } else if (strcmp(token.string, "begin") == 0) {
      token.type = T_BEGIN;
    } else if (strcmp(token.string, "while") == 0) {
      token.type = T_WHILE;
    } else if (strcmp(token.string, "elsif") == 0) {
      token.type = T_ELSIF;
    } else if (strcmp(token.string, "return") == 0) {
      token.type = T_RETURN;
    } else if (strcmp(token.string, "program") == 0) {
      token.type = T_PROGRAM;
    } else if (strcmp(token.string, "procedure") == 0) {
      token.type = T_PROCEDURE;
    } else if (strcmp(token.string, "function") == 0) {
      token.type = T_FUNCTION;
    } else {
      token.type = T_ID;
    }
    return token;
  }

  // Check for numbers
  if (isdigit(text[pos])) {
    int start = pos;
    while (isdigit(text[pos])) {
      pos++;
    }
    int length = pos - start;
    token.string = malloc(length + 1);
    strncpy(token.string, &text[start], length);
    token.string[length] = '\0';
    token.type = T_INTNUM;
    if (text[pos] == '.') {
      pos++;
      while (isdigit(text[pos])) {
        pos++;
      }
      token.type = T_REALNUM;
    }
    return token;
  }

  // Return error token if none of the above cases are met
  token.type = T_ERROR;
  return token;
}

int main() {
  printf("Enter an expression: ");
  scanf("%s", text);

  Token token = get_next_token();
  while (token.type != T_EOF) {
    printf("Token: type=%d, value=%d, string=%s\n", token.type, token.value, token.string);
    token = get_next_token();
  }

  return 0;
}