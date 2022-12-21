#include "lexer.h"
#include "parser.h"

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
    return lookup_symbol(name)->type;
}

void parse() {
  Token token = get_next_token();
  while (token.type != T_EOF) {
    //TODO
  }
}


int main(int argc, char* argv[]) {
  printf("Enter an expression: ");
  scanf("%s", text);
  parse();

  return 0;
}
