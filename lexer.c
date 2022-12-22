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
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (text[pos] == token_map[i].c) {
      token.type = token_map[i].type;
      pos++;
      return token;
    }
  }

  // Check for reserved words and identifiers
  if (isalpha(text[pos])) {
    int start = pos;
    while (isalnum(text[pos])) {
      pos++;
    }
    int length = pos - start;

    // Check if the string is a reserved word
    for (int i = 0; i < RESERVED_WORD_COUNT; i++) {
      if (strncmp(&text[start], reserved_words[i].word, length) == 0) {
        token.type = reserved_words[i].type;
        add_symbol(reserved_words[i].word, token.type, 0);
        return token;
      }
    }

    // If not a reserved word, it is an identifier
    strncpy(token_string_buffer, &text[start], length);
    token_string_buffer[length] = '\0';
    token.type = T_ID;
    token.string = token_string_buffer;
    add_symbol(token.string, token.type, 0);
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

Symbol* lookup_symbol(char *name) {
  for (int i = 0; i < num_symbols; i++) {
    if (strcmp(symbol_table[i].name, name) == 0) {
      return &symbol_table[i];
    }
  }
  return NULL;
}

void add_symbol(char *name, int type, int value) {
  if(!lookup_symbol(name)) {
    symbol_table[num_symbols].name = strdup(name);
    symbol_table[num_symbols].type = type;
    symbol_table[num_symbols].value = value;
    num_symbols++;
  }
}

void add_token(int type, int value, char* str) {
  if (num_tokens == MAX_TOKENS) {
    // Error: token array is full
    return;
  }
  tokens[num_tokens].type = type;
  tokens[num_tokens].value = value;
  tokens[num_tokens].string = strdup(str);
  num_tokens++;
}

Token *get_token(int index) {
  if (index < 0 || index >= num_tokens) {
    // Error: index out of range
    return NULL;
  }
  return &tokens[index];
}

void print_symbol_table(Symbol *symbol_table, int num_symbols) {
  printf("Symbol Table:\n");
  printf("-------------\n");
  for (int i = 0; i < num_symbols; i++) {
    Symbol symbol = symbol_table[i];
    printf("%s\t%d\t%d\n", symbol.name, symbol.type, symbol.value);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error: Could not open file '%s'\n", filename);
    return 1;
  }

  // Read the input text from the file
  char line[100];
  while (fgets(line, sizeof(line), file)) {
    strcat(text, line);
  }

  fclose(file);

  Token token = get_next_token();
  while (token.type != T_EOF) {
    printf("Token: type=%d, value=%d, string=%s\n", token.type, token.value, token.string);
    token = get_next_token();
  }

  print_symbol_table(symbol_table, num_symbols);

  return 0;
}
