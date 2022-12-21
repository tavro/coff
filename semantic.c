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

int main() {
  add_symbol("x", 1, 10);
  add_symbol("y", 2, 20);
  add_symbol("z", 3, 30);

  Symbol *sym = lookup_symbol("y");
  if (sym != NULL) {
    printf("Found symbol: name=%s, type=%d, value=%d\n", sym->name, sym->type, sym->value);
  } else {
    printf("Symbol not found\n");
  }

  return 0;
}
