#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

/*
 *
 * Please be aware that at this point this is only a playground for me
 * so the code will not look pretty.
 *
 */
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

/*
 *
 * Parses variable declaration 'var [type] [name];'
 * Returns: Ast Node of type VARDEC with left child type & right child id
 *
 */
AstNode* parse_var(int* index) {
  (*index)+=2; // skip 'var' & 'type'

  AstNode* var = malloc(sizeof(AstNode));
  var->type = AST_VARDEC;

  AstNode* id = malloc(sizeof(AstNode));
  id->type = AST_ID;

  AstNode* type = malloc(sizeof(AstNode));
  type->type = AST_TYPE;

  id->data.value.char_val = tokens[(*index)--].string; // go back to 'type'
  switch(tokens[(*index)].type) { 
    case T_INTTYPE:
      lookup_symbol(id->data.value.char_val)->sym_type = S_INT;
      type->data.value.char_val = "int";
      break;
    case T_REALTYPE:
      lookup_symbol(id->data.value.char_val)->sym_type = S_REAL;
      type->data.value.char_val = "real";
      break;
    case T_BOOLTYPE:
      lookup_symbol(id->data.value.char_val)->sym_type = S_BOOL;
      type->data.value.char_val = "bool";
      break;
    case T_STRTYPE:
      lookup_symbol(id->data.value.char_val)->sym_type = S_STR;
      type->data.value.char_val = "string";
      break;
    default:
      lookup_symbol(id->data.value.char_val)->sym_type = S_VOID;
      type->data.value.char_val = "void";
      break;
  }

  var->left = type;
  var->right = id;

  (*index)+=2; // skip 'type' & 'name'

  return var;
}

/*
 *
 * Parses parameters '(arg [type] [name], ...)'
 * Returns: Ast Node of type PARAMLIST with children of type PARAM. 
 * Each PARAM node has left child type & right child id.
 *
 */
AstNode* parse_params(int* index) {
  AstNode* paramlist = malloc(sizeof(AstNode));
  paramlist->type = AST_PARAMLIST;
  paramlist->children = NULL;
  paramlist->num_children = 0;
  
  (*index)++; // skip '('
  while(tokens[(*index)].type != T_RIGHTPAR) {
    if(tokens[(*index)].type == T_COMMA) {
      (*index)++; // skip ','
    }

    AstNode* param = malloc(sizeof(AstNode));
    param->type = AST_PARAM;
    (*index)++; // skip 'arg'

    AstNode* type = malloc(sizeof(AstNode));
    type->type = AST_TYPE;

    AstNode* id = malloc(sizeof(AstNode));
    id->type = AST_ID;
    id->data.value.char_val = tokens[++(*index)].string; // skip 'type'
    switch(tokens[--(*index)].type) { // go back to 'type'
      case T_INTTYPE:
        lookup_symbol(id->data.value.char_val)->sym_type = S_INT;
        type->data.value.char_val = "int";
        break;
      case T_REALTYPE:
        lookup_symbol(id->data.value.char_val)->sym_type = S_REAL;
        type->data.value.char_val = "real";
        break;
      case T_BOOLTYPE:
        lookup_symbol(id->data.value.char_val)->sym_type = S_BOOL;
        type->data.value.char_val = "bool";
        break;
      case T_STRTYPE:
        lookup_symbol(id->data.value.char_val)->sym_type = S_STR;
        type->data.value.char_val = "string";
        break;
      default:
        lookup_symbol(id->data.value.char_val)->sym_type = S_VOID;
        type->data.value.char_val = "void";
        break;
    }
    
    param->left = type;
    param->right = id;

    add_child_node(paramlist, param);

    (*index)+=2; // skip 'type' & 'name'
  }

  (*index)++; // skip ')'
  return paramlist;
}

/*
 *
 * Parses function definitions 'func [type] (arg [type] [paramname], ...) [funcname]:'
 * Returns: Ast Node of type FUNCDEC with left child params & right child return
 *
 */
AstNode* parse_function(int* index) {
  AstNode* function = malloc(sizeof(AstNode));
  function->type = AST_FUNCDEC;
  function->children = NULL;
  function->num_children = 0;

  AstNode* functype = malloc(sizeof(AstNode));
  functype->type = AST_TYPE;
  int int_type = 0;
  switch(tokens[++(*index)].type) { // skip 'func'
      case T_INTTYPE:
        functype->data.value.char_val = "int";
        int_type = 1;
        break;
      case T_REALTYPE:
        functype->data.value.char_val = "real";
        int_type = 2;
        break;
      case T_BOOLTYPE:
        functype->data.value.char_val = "bool";
        int_type = 3;
        break;
      case T_STRTYPE:
        functype->data.value.char_val = "string";
        int_type = 4;
        break;
      default:
        functype->data.value.char_val = "void";
        break;
  }

  add_child_node(function, functype);
  (*index)++; // skip 'type'

  function->left = parse_params(index);

  AstNode* funcid = malloc(sizeof(AstNode));
  funcid->type = AST_ID;
  funcid->data.value.char_val = tokens[(*index)].string;

  switch(int_type) {
    case 1:
      lookup_symbol(funcid->data.value.char_val)->sym_type = S_INT;
      break;
    case 2:
      lookup_symbol(funcid->data.value.char_val)->sym_type = S_REAL;
      break;
    case 3:
      lookup_symbol(funcid->data.value.char_val)->sym_type = S_BOOL;
      break;
    case 4:
      lookup_symbol(funcid->data.value.char_val)->sym_type = S_STR;
      break;
    default:
      lookup_symbol(funcid->data.value.char_val)->sym_type = S_VOID;
      break;
  }

  add_child_node(function, funcid);
  (*index)++; // skip :

  AstNode* ret = malloc(sizeof(AstNode));
  ret->type = AST_RETURN;

  (*index)+=2; // skip return & get return token

  AstNode* node = malloc(sizeof(AstNode));
  switch(tokens[(*index)].type) {
    case T_INTNUM:
      node->type = AST_INT;
      node->data.value.int_val = tokens[(*index)].ival;
      break;
    case T_STR:
      node->type = AST_STR;
      node->data.value.char_val = tokens[(*index)].string;
      break;
    case T_ID:
      node->type = AST_ID;
      node->data.value.char_val = tokens[(*index)].string;
      break;
    case T_TRUE:
    case T_FALSE:
      node->type = AST_BOOL;
      node->data.value.int_val = tokens[(*index)].ival;
      break;
    case T_REALNUM:
      node->type = AST_REAL;
      node->data.value.real_val = tokens[(*index)].rval;
      break;
    default:
      break;
  }
  ret->right = node;

  ret->left = NULL;
  function->right = ret;

  return function;
}

/*
 *
 * Parses program definition 'program [name]:'
 * Returns: Ast Node of type PROGRAM with program id as left child
 *
 */
AstNode* parse_program(int* index) {
  AstNode* program = malloc(sizeof(AstNode));
  program->type = AST_PROGRAM;
  
  AstNode* prgid = malloc(sizeof(AstNode));
  prgid->type = AST_ID;
  prgid->data.value.char_val = tokens[++(*index)].string; // program id
  
  program->left = prgid;
  program->right = NULL;
  program->children = NULL;
  program->num_children = 0;

  while (*index < num_tokens) {
    (*index)++;

    if (tokens[*index].type == T_VAR) { // Parse variable
      add_child_node(program, parse_var(index));
    }
    else if (tokens[*index].type == T_FUNCTION) { // Parse function
      add_child_node(program, parse_function(index));
    }
    else if (tokens[*index].type == T_ID) { // Parse assignment and calls
      Symbol* sym = lookup_symbol(tokens[*index].string);
      (*index)++;

      if(tokens[*index].type == T_ASSIGN) { // Parse assignment
        AstNode* assignment = malloc(sizeof(AstNode));
        assignment->type = AST_ASSIGN;

        AstNode* lhs = malloc(sizeof(AstNode));
        lhs->type = AST_ID;
        lhs->data.value.char_val = tokens[--(*index)].string;
        (*index)++;

        AstNode* rhs = malloc(sizeof(AstNode));
        switch(tokens[++(*index)].type) {
          case T_INTNUM:
            rhs->type = AST_INT;
            rhs->data.value.int_val = tokens[(*index)].ival;
            sym->value = rhs->data.value.int_val;
            break;
          case T_STR:
            rhs->type = AST_STR;
            rhs->data.value.char_val = tokens[(*index)].string;
            sym->str_val = rhs->data.value.char_val;
            break;
          case T_ID:
            rhs->type = AST_ID;
            rhs->data.value.char_val = tokens[(*index)].string;
            sym->str_val = rhs->data.value.char_val;
            break;
          case T_TRUE:
          case T_FALSE:
            rhs->type = AST_BOOL;
            rhs->data.value.int_val = tokens[(*index)].ival;
            sym->value = rhs->data.value.int_val;
            break;
          case T_REALNUM:
            rhs->type = AST_REAL;
            rhs->data.value.real_val = tokens[(*index)].rval;
            sym->rval = rhs->data.value.real_val;
            break;
          default:
            break;
        }

        assignment->left = lhs;  // Id
        assignment->right = rhs; // Value
        
        add_child_node(program, assignment);
      }
      else if(tokens[*index].type == T_LEFTPAR) { // Parse function call
        AstNode* call = malloc(sizeof(AstNode));
        call->type = AST_FUNCCALL;

        AstNode* id = malloc(sizeof(AstNode));
        id->type = AST_ID;
        id->data.value.char_val = tokens[--(*index)].string;

        (*index)++; // go back to '('

        AstNode* arglist = malloc(sizeof(AstNode));
        arglist->type = AST_ARGLIST;
        arglist->children = NULL;
        arglist->num_children = 0;

        ++(*index); // skip '('
        while(tokens[(*index)].type != T_RIGHTPAR) {
          AstNode* arg = malloc(sizeof(AstNode));
          arg->type = AST_ARG;
          AstNode* argument_type = malloc(sizeof(AstNode));

          switch(tokens[(*index)].type) {
            case T_INTNUM:
              argument_type->type = AST_INT;
              argument_type->data.value.int_val = tokens[(*index)].ival;
              break;
            case T_STR:
              argument_type->type = AST_STR;
              argument_type->data.value.char_val = tokens[(*index)].string;
              break;
            case T_ID:
              argument_type->type = AST_ID;
              argument_type->data.value.char_val = tokens[(*index)].string;
              break;
            case T_TRUE:
            case T_FALSE:
              argument_type->type = AST_BOOL;
              argument_type->data.value.int_val = tokens[(*index)].ival;
              break;
            case T_REALNUM:
              argument_type->type = AST_REAL;
              argument_type->data.value.real_val = tokens[(*index)].rval;
              break;
            default:
              break;
          }

          ++(*index);
          if(tokens[(*index)].type == T_COMMA) {
            ++(*index); // skip ','
          }

          arg->right = argument_type;
          add_child_node(arglist, arg);
        }
        (*index)--; // go back from ')'

        call->left = id;  // AST_ID
        call->right = arglist; // AST_ARGLIST

        add_child_node(program, call);
      }
    } 
    else if (tokens[*index].type == T_PRINT) { // Parse print
      (*index)+=2; // skip 'print' & '('

      AstNode* print = malloc(sizeof(AstNode));
      print->type = AST_PRINT;

      AstNode* child = malloc(sizeof(AstNode));
      switch(tokens[(*index)].type) {
        case T_INTNUM:
          child->type = AST_INT;
          child->data.value.int_val = tokens[(*index)].ival;
          break;
        case T_STR:
          child->type = AST_STR;
          child->data.value.char_val = tokens[(*index)].string;
          break;
        case T_ID:
          child->type = AST_ID;
          child->data.value.char_val = tokens[(*index)].string;
          break;
        case T_TRUE:
        case T_FALSE:
          child->type = AST_BOOL;
          child->data.value.int_val = tokens[(*index)].ival;
          break;
        case T_REALNUM:
          child->type = AST_REAL;
          child->data.value.real_val = tokens[(*index)].rval;
          break;
        default:
          break;
      }
      print->left = child;
      (*index)++;

      print->right = NULL;
      add_child_node(program, print);
    }
  }

  return program;
}

// TODO: Refactor
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
    case AST_PARAMLIST:
      printf("%sAST_PARAMLIST:\n", indent);
      for (int i = 0; i < node->num_children; i++) {
        AstNode* child = node->children[i];
        print_ast(child, "            ");
      }
      break;
    case AST_PARAM:
      printf("%sAST_PARAM:\n", indent);
      if(node->left != NULL)
        print_ast(node->left, "                ");
      if(node->right != NULL)
        print_ast(node->right, "                ");
      break;
    case AST_ARGLIST:
      printf("%sAST_ARGLIST:\n", indent);
      for (int i = 0; i < node->num_children; i++) {
        AstNode* child = node->children[i];
        print_ast(child, "            ");
      }
      break;
    case AST_ARG:
      printf("%sAST_ARG:\n", indent);
      if(node->right != NULL)
        print_ast(node->right, "                ");
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

// TODO: Move to utils header
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

// TODO: Refactor
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

  int index = 0;
  AstNode* root = parse_program(&index);
  print_ast(root, "");

  print_symbol_table(symbol_table, num_symbols);
  write_symbol_table("output/parser-symtab-out.txt", symbol_table, num_symbols);

  return 0;
}