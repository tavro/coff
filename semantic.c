#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *name;
  int type;
  int value;
} Symbol;

Symbol symbol_table[100];
int num_symbols = 0;

void add_symbol(char *name, int type, int value) {
  symbol_table[num_symbols].name = strdup(name);
  symbol_table[num_symbols].type = type;
  symbol_table[num_symbols].value = value;
  num_symbols++;
}

Symbol* lookup_symbol(char *name) {
  for (int i = 0; i < num_symbols; i++) {
    if (strcmp(symbol_table[i].name, name) == 0) {
      return &symbol_table[i];
    }
  }
  return NULL;
}

int get_type(char *name) {
    return lookup_symbol(name).type;
}

void semantic_analyzer() {
    Token token = get_next_token();
    while (token.type != T_EOF) {
        //TODO
    }
}

int main() {
  
  return 0;
}
