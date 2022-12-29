#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

Symbol* lookup_symbol(char *name) {
  for (int i = 0; i < num_symbols; i++) {
    if (strcmp(symbol_table[i].name, name) == 0) {
      return &symbol_table[i];
    }
  }
  return NULL;
}

void add_child_node(AstNode* parent, AstNode* child) {
  if (parent->children == NULL) {
    parent->children = malloc(sizeof(AstNode*));
    if (parent->children == NULL) {
      fprintf(stderr, "Error allocating memory for child node\n");
      exit(1);
    }
    parent->num_children = 0;
  } else {
    parent->children = realloc(parent->children, (parent->num_children + 1) * sizeof(AstNode*));
    if (parent->children == NULL) {
      fprintf(stderr, "Error allocating memory for child node\n");
      exit(1);
    }
  }
  
  parent->children[parent->num_children] = child;
  parent->num_children++;
}

// var type name;
AstNode* parse_var(int* index) {
  AstNode* var = malloc(sizeof(AstNode));
  var->type = AST_VARDEC;

  if(tokens[++(*index)].type == T_INTTYPE) {
    var->val_type = "INT_TYPE";
  }

  AstNode* id = malloc(sizeof(AstNode));
  id->type = AST_ID;
  id->data.value.char_val = tokens[++(*index)].string;

  //var->data.value.char_val = tokens[++(*index)].string;

  if(strcmp(var->val_type, "INT_TYPE") == 0) {
    lookup_symbol(id->data.value.char_val)->sym_type = S_INT;
  }

  AstNode* type = malloc(sizeof(AstNode));
  type->type = AST_TYPE;
  type->data.value.char_val = "int"; 

  var->left = type;
  var->right = id;

  (*index)++; // skip ;

  return var;
}

AstNode* parse_arguments(int* index) {
  (*index)++; // skip (
  
  AstNode* argument = malloc(sizeof(AstNode));
  argument->type = AST_PARAM;
  (*index)++; // skip arg

  if(tokens[++(*index)].type == T_INTTYPE) {
    argument->val_type = "INT_TYPE";
  }

  AstNode* type = malloc(sizeof(AstNode));
  type->type = AST_TYPE;
  type->data.value.char_val = "int"; 

  AstNode* id = malloc(sizeof(AstNode));
  id->type = AST_ID;
  id->data.value.char_val = tokens[++(*index)].string;
  //argument->data.value.char_val = tokens[++(*index)].string;

  if(strcmp(argument->val_type, "INT_TYPE") == 0) {
    lookup_symbol(id->data.value.char_val)->sym_type = S_INT;
  }

  argument->left = type;
  argument->right = id;

  (*index)++; // skip )
  return argument;
}

// func int (arg int bizz) bar:
AstNode* parse_function(int* index) {
  AstNode* function = malloc(sizeof(AstNode));
  function->type = AST_FUNCDEC;

  function->children = NULL;
  function->num_children = 0;

  if(tokens[++(*index)].type == T_INTTYPE) {
    function->val_type = "INT_TYPE";
  }

  AstNode* functype = malloc(sizeof(AstNode));
  functype->type = AST_TYPE;
  functype->data.value.char_val = "int";
  add_child_node(function, functype);

  function->left = parse_arguments(index);

  AstNode* funcid = malloc(sizeof(AstNode));
  funcid->type = AST_ID;
  funcid->data.value.char_val = tokens[++(*index)].string;
  add_child_node(function, funcid);

  //function->data.value.char_val = tokens[++(*index)].string;

  (*index)++; // skip :
  AstNode* ret = malloc(sizeof(AstNode));
  ret->type = AST_RETURN;
  (*index)++; // skip return

  AstNode* id = malloc(sizeof(AstNode));
  id->type = AST_ID;
  id->data.value.char_val = tokens[++(*index)].string;
  
  ret->right = id;
  //ret->data.value.char_val = tokens[++(*index)].string;

  if(strcmp(function->val_type, "INT_TYPE") == 0) {
    lookup_symbol(funcid->data.value.char_val)->sym_type = S_INT;
  }

  ret->left = NULL;

  function->right = ret;

  return function;
}

// program name:;
AstNode* parse_program(int* index) {
  AstNode* program = malloc(sizeof(AstNode));
  program->type = AST_PROGRAM;
  //program->data.value.char_val = tokens[++(*index)].string; //program id
  
  AstNode* prgid = malloc(sizeof(AstNode));
  prgid->type = AST_ID;
  prgid->data.value.char_val = tokens[++(*index)].string;
  program->left = prgid;

  program->right = NULL;
  program->children = NULL;
  program->num_children = 0;

  while (*index < num_tokens) {
    (*index)++;
    if (tokens[*index].type == T_VAR) {
      add_child_node(program, parse_var(index));
    } else if (tokens[*index].type == T_FUNCTION) {
      add_child_node(program, parse_function(index));
    } else if (tokens[*index].type == T_ID) {
      Symbol* s = lookup_symbol(tokens[*index].string);
      (*index)++;
      if(tokens[*index].type == T_ASSIGN) {
        AstNode* assignment = malloc(sizeof(AstNode));
        assignment->type = AST_ASSIGN;

        AstNode* lhs = malloc(sizeof(AstNode));
        lhs->type = AST_ID;
        lhs->data.value.char_val = tokens[--(*index)].string;
        //assignment->val_type = tokens[--(*index)].string;
        (*index)++;

        AstNode* rhs = malloc(sizeof(AstNode));
        rhs->type = AST_INT;
        rhs->data.value.int_val = tokens[++(*index)].ival;
        //assignment->data.value.int_val = tokens[++(*index)].ival;
        //s->value = assignment->data.value.int_val;
        s->value = rhs->data.value.int_val;

        assignment->left = lhs;  // AST_ID
        assignment->right = rhs; // AST_INT
        
        add_child_node(program, assignment);
      }
      else if(tokens[*index].type == T_LEFTPAR) {
        AstNode* call = malloc(sizeof(AstNode));
        call->type = AST_FUNCCALL;

        AstNode* id = malloc(sizeof(AstNode));
        id->type = AST_ID;
        id->data.value.char_val = tokens[--(*index)].string;

        (*index)++;

        AstNode* arg = malloc(sizeof(AstNode));
        arg->type = AST_ARG;
        AstNode* integer = malloc(sizeof(AstNode));
        integer->type = AST_INT;
        integer->data.value.int_val = tokens[++(*index)].ival;
        arg->right = integer;

        call->left = id;  // AST_ID
        call->right = arg; // AST_ARG

        add_child_node(program, call);
      }
    } else if (tokens[*index].type == T_PRINT) {
      (*index)+=2;
      AstNode* print = malloc(sizeof(AstNode));
      print->type = AST_PRINT;

      AstNode* child = malloc(sizeof(AstNode));
      child->type = AST_ID;
      child->data.value.char_val = tokens[(*index)++].string;

      print->left = child;
      print->right = NULL;
      add_child_node(program, print);
    }
  }

  return program;
}

AstNode *parse_factor(Token token) {
  AstNode *node;
  switch (token.type) {
    case T_INTNUM:
      node = malloc(sizeof(AstNode));
      node->type = AST_INT;
      node->data.value.int_val = token.ival;
      break;
    case T_REALNUM:
      node = malloc(sizeof(AstNode));
      node->type = AST_REAL;
      node->data.value.real_val = token.rval;
      break;
    default:
      fprintf(stderr, "Unexpected token '%d'\n", token.type);
      exit(1);
  }
  return node;
}

void print_ast(AstNode *node, char* indent) {
  switch (node->type) {
    case AST_INT:
      printf("%sAST_INT: %d\n", indent, node->data.value.int_val);
      break;
    case AST_ID:
      printf("%sAST_ID: %s\n", indent, node->data.value.char_val);
      break;
    case AST_TYPE:
      printf("%sAST_TYPE: %s\n", indent, node->data.value.char_val);
      break;
    case AST_REAL:
      printf("AST_REAL: %f\n", node->data.value.real_val);
      break;
    case AST_PROGRAM:
      printf("AST_PROGRAM:\n");
      if(node->left != NULL)
        print_ast(node->left, "    ");
      // Iterate over the child nodes
      for (int i = 0; i < node->num_children; i++) {
        AstNode* child = node->children[i];
        print_ast(child, "    ");
      }
      break;
    case AST_VARDEC:
      printf("%sAST_VARDEC:\n", indent);
      if(node->left != NULL)
        print_ast(node->left, "        ");
      if(node->right != NULL)
        print_ast(node->right, "        ");
      break;
    case AST_FUNCDEC:
      printf("%sAST_FUNCDEC:\n", indent);
      for (int i = 0; i < node->num_children; i++) {
        AstNode* child = node->children[i];
        print_ast(child, "        ");
      }
      if(node->left != NULL)
        print_ast(node->left, "        ");
      if(node->right != NULL)
        print_ast(node->right, "        ");
      break;
    case AST_PARAM:
      printf("%sAST_PARAM:\n", indent);
      if(node->left != NULL)
        print_ast(node->left, "            ");
      if(node->right != NULL)
        print_ast(node->right, "            ");
      break;
    case AST_ARG:
      printf("%sAST_ARG:\n", indent);
      if(node->right != NULL)
        print_ast(node->right, "            ");
      break;
    case AST_RETURN:
      printf("%sAST_RETURN:\n", indent);
      if(node->right != NULL)
        print_ast(node->right, "            ");
      break;
    case AST_PRINT:
      printf("%sAST_PRINT:\n", indent);
      print_ast(node->left, "        ");
      break;
    case AST_ASSIGN:
      printf("%sAST_ASSIGN:\n", indent);
      if(node->left != NULL)
        print_ast(node->left, "        ");
      if(node->right != NULL)
        print_ast(node->right, "        ");
      break;
    case AST_FUNCCALL:
      printf("%sAST_FUNCCALL:\n", indent);
      if(node->left != NULL)
        print_ast(node->left, "        ");
      if(node->right != NULL)
        print_ast(node->right, "        ");
      break;
    default:
      fprintf(stderr, "Unexpected AST type '%d'\n", node->type);
      exit(1);
  }
}

void add_symbol(char *name, int type, int value) {
  if(!lookup_symbol(name)) {
    symbol_table[num_symbols].name = strdup(name);
    symbol_table[num_symbols].tok_type = type;
    symbol_table[num_symbols].sym_type = S_NONE;
    symbol_table[num_symbols].value = value;
    num_symbols++;
  }
}

void read_symbol_table(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Error opening file");
    return;
  }

  char line[256];
  for (int i = 0; i < 5; i++) {
    fgets(line, sizeof(line), fp);
  }
  while (fgets(line, sizeof(line), fp)) {
    if (line[0] == '\n') {
      break;
    }
    char name[32];
    int t_type, s_type, value;
    sscanf(line, "%s %d=T_%*s %d=S_%*s %d", name, &t_type, &s_type, &value);
    add_symbol(name, t_type, value);
  }

  fclose(fp);
}

int main(int argc, char **argv) {
  read_symbol_table("./output/lexer-symtab-out.txt");

  FILE* input_file = fopen("./output/lexer-tokens-out.txt", "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Unable to open input file\n");
    return 1;
  }

  char line[1024];

  while (fgets(line, sizeof(line), input_file) != NULL) {
    Token token;
    sscanf(line, "Token(%d, %d): type=%d(%*s) ival=%*d rval=%*f string=%*s", &token.col, &token.row, &token.type);

    char* ival_str = strstr(line, "ival=");
    char* rval_str = strstr(line, "rval=");

    char* sval_str = strstr(line, "string=");
    if(strstr(sval_str, "string=(null)") == NULL) {
      sval_str = sval_str+7;
      sval_str[strlen(sval_str)-1] = '\0';
      token.string = strdup(sval_str);
    }
    else {
      token.string = strdup("(null)");
    }

    if (ival_str != NULL) {
      token.ival = (int)strtol(ival_str + 5, NULL, 10);
    }
    if (rval_str != NULL) {
      token.rval = strtod(rval_str + 5, NULL);
    }
    
    tokens[num_tokens++] = token;
  }
  fclose(input_file);

  printf("\n");
  printf("=============================\n");
  printf("Array of Tokens\n");
  printf("=============================\n");
  printf("pos        type        ival    rval        string\n");
  printf("-----------------------------\n");
  for (int i = 0; i < num_tokens; i++) {
    Token token = tokens[i];
    printf("(%-2d, %-2d):  %-12d%-8d%-12f%s\n", token.col, token.row, token.type, token.ival, token.rval, token.string);
  }

  /*
  for (int i = 0; i < num_tokens; i++) {
    Token token = tokens[i];
    if(token.type == T_INTNUM || token.type == T_REALNUM) {
      AstNode *ast = parse_factor(token);
      print_ast(ast, "");
    }
  }
  */

  int index = 0;
  AstNode* root = parse_program(&index);
  print_ast(root, "");

  print_symbol_table(symbol_table, num_symbols);
  write_symbol_table("output/parser-symtab-out.txt", symbol_table, num_symbols);

  return 0;
}