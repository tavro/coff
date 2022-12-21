typedef struct {
  char *name;
  int type;
  int value;
} Symbol;

Symbol symbol_table[100];
int num_symbols = 0;

void add_symbol(char *name, int type, int value);

Symbol* lookup_symbol(char *name);

int get_type(char *name);