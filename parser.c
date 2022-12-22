#include "parser.h"

AstNode* createAstNode(int type, int intval, char* strval) {
  AstNode* node = (AstNode*) malloc(sizeof(AstNode));
  node->type = type;
  node->intval = intval;
  node->strval = strval;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void printAst(AstNode* root) {
  printf("%d\n", root->type);

  if (root->left != NULL) {
    printAst(root->left);
  }
  if (root->right != NULL) {
    printAst(root->right);
  }
}

AstNode* parseTokens(Token* tokens, int numTokens) {
  AstNode* node = NULL;
  int currentToken = 0;

  while (currentToken < numTokens) {
    Token current = tokens[currentToken];
    switch (current.type) {
      case T_PROGRAM:
        node = createAstNode(AST_PROGRAM, current.value, current.string);
        currentToken++;
        break;
      case T_ID:
        currentToken++;
        break;
      case T_COLON:
        currentToken++;
        break;
      case T_VAR:
        // create a new AST_VAR node and set it as the left child of the current node
        node->left = createAstNode(AST_VARIABLE, current.value, current.string);
        currentToken++;
        break;
      case T_INT:
        // set the type of the left child of the current node to AST_INT
        currentToken++;
        break;
      case T_SEMICOLON:
        // move on to the next statement
        currentToken++;
        break;
      case T_FUNCTION:
        // create a new AST_FUNCTION node and set it as the right child of the current node
        node->right = createAstNode(AST_FUNCTION, current.value, current.string);
        currentToken++;
        break;
      case T_LEFTPAR:
        currentToken++;
        break;
      case T_RIGHTPAR:
        currentToken++;
        break;
      default:
        // handle other token types as needed
        currentToken++;
        break;
    }
  }

  return node;
}

int main(int argc, char **argv) {
  int numTokens = sizeof(tokenseq) / sizeof(Token);
  AstNode* root = parseTokens(tokenseq, numTokens);
  printAst(root);

  return 0;
}