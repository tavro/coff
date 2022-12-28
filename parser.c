#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

Token tokens[999];
int num_tokens = 0;

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
  var->type = AST_VAR;

  if(tokens[++(*index)].type == T_INTTYPE) {
    var->val_type = "INT_TYPE";
  }

  var->data.value.char_val = tokens[++(*index)].string;
  var->left = NULL;
  var->right = NULL;

  (*index)++; // skip ;

  return var;
}

AstNode* parse_arguments(int* index) {
  (*index)++; // skip (
  
  AstNode* argument = malloc(sizeof(AstNode));
  argument->type = AST_ARG;
  (*index)++; // skip arg

  if(tokens[++(*index)].type == T_INTTYPE) {
    argument->val_type = "INT_TYPE";
  }

  argument->data.value.char_val = tokens[++(*index)].string;
  argument->left = NULL;
  argument->right = NULL;

  (*index)++; // skip )
  return argument;
}

// func int (arg int bizz) bar:
AstNode* parse_function(int* index) {
  AstNode* function = malloc(sizeof(AstNode));
  function->type = AST_FUNC;

  if(tokens[++(*index)].type == T_INTTYPE) {
    function->val_type = "INT_TYPE";
  }

  function->left = parse_arguments(index);

  function->data.value.char_val = tokens[++(*index)].string;

  (*index)++; // skip :
  AstNode* ret = malloc(sizeof(AstNode));
  ret->type = AST_RETURN;
  (*index)++; // skip return
  ret->data.value.char_val = tokens[++(*index)].string;
  ret->left = NULL;
  ret->right = NULL;

  function->right = ret;

  return function;
}

// program name:;
AstNode* parse_program(int* index) {
  AstNode* program = malloc(sizeof(AstNode));
  program->type = AST_PROGRAM;
  program->data.value.char_val = tokens[++(*index)].string; //program id
  program->left = NULL;
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
      (*index)++;
      if(tokens[*index].type == T_ASSIGN) {
        AstNode* assignment = malloc(sizeof(AstNode));
        assignment->type = AST_ASSIGN;
        assignment->val_type = tokens[--(*index)].string;
        (*index)++;
        assignment->data.value.int_val = tokens[++(*index)].ival;
        assignment->left = NULL;
        assignment->right = NULL;
        add_child_node(program, assignment);
      }
    }
  }

  return program;
}

AstNode *parse_factor(Token token) {
  AstNode *node;
  switch (token.type) {
    case T_INTNUM:
      node = malloc(sizeof(AstNode));
      node->type = AST_INTEGER;
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
    case AST_INTEGER:
      printf("AST_INTEGER: %d\n", node->data.value.int_val);
      break;
    case AST_REAL:
      printf("AST_REAL: %f\n", node->data.value.real_val);
      break;
    case AST_PROGRAM:
      printf("AST_PROGRAM: %s\n", node->data.value.char_val);
      // Iterate over the child nodes
      for (int i = 0; i < node->num_children; i++) {
        AstNode* child = node->children[i];
        print_ast(child, "    ");
      }
      break;
    case AST_VAR:
      printf("%sAST_VAR (%s): %s\n", indent, node->val_type, node->data.value.char_val);
      break;
    case AST_FUNC:
      printf("%sAST_FUNCTION (%s): %s\n", indent, node->val_type, node->data.value.char_val);
      if(node->left != NULL)
        print_ast(node->left, "        ");
      if(node->right != NULL)
        print_ast(node->right, "        ");
      break;
    case AST_ARG:
      printf("%sAST_ARG (%s): %s\n", indent, node->val_type, node->data.value.char_val);
      break;
    case AST_RETURN:
      printf("%sAST_RETURN: %s\n", indent, node->data.value.char_val);
      break;
    case AST_ASSIGN:
      printf("%sAST_ASSIGN: %s=%d\n", indent, node->val_type, node->data.value.int_val);
      break;
    default:
      fprintf(stderr, "Unexpected AST type '%d'\n", node->type);
      exit(1);
  }
}

int main(int argc, char **argv) {
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

  return 0;
}