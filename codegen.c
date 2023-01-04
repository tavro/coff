void generate_code(AstNode *root) {
    switch (root->type) {
        case AST_PROGRAM:
            printf("#include <stdio.h>\n\n");
            printf("int main() {\n");
            
            for (int i = 0; i < root->num_children; i++) { // Generate code for children nodes
                AstNode* child = root->children[i];
                generate_code(child);
            }

            printf("\n\treturn 0;\n}\n");
            break;
        case AST_VARDEC:
            generate_code(root->left); // Generate code for type
            printf(" %s;\n", root->right->data.value.string_val);
            break;
        case AST_FUNCDEC:
            generate_code(root->children[0]); // Generate code for type
            generate_code(root->children[1]); // Generate code for id
            printf(" (");
            generate_code(root->left);
            printf(") {\n");
            generate_code(root->right); // TODO: Implement AST_RETURN
            printf("\n}\n");
            break;
        case AST_ASSIGN:
            printf("\t%s = ", root->left->data.value.char_val);
            generate_code(root->right); // Generate code for constant or id
            printf(";\n");
            break;
        case AST_FUNCCALL:
            printf("\t%s(", root->left->data.value.char_val);
            generate_code(root->right); // Generate code for argument list
            printf(");\n");
            break;
        case AST_PRINT:
            printf("\tprintf(\"%%d\\n\", ");
            generate_code(root->left);
            printf(");\n");
            break;
        case AST_TYPE:
        case AST_ID:
            printf("%s", root->data.value.char_val);
            break;
        case AST_INT:
            printf("%d", root->data.value.int_val);
            break;
        case AST_PARAMLIST:
        case AST_ARGLIST:
            for (int i = 0; i < root->num_children; i++) {
                AstNode* child = root->children[i];
                if(i != (root->num_children - 1))
                    printf(", ");
                generate_code(child);
            }
            break;
        case AST_PARAM:
            generate_code(root->left); // Generate code for type
            printf(" %s", root->right->data.value.string_val);
            break;
        case AST_ARG:
            generate_code(root->right);
            break;
        default:
            printf("// [%d]: NOT YET IMPLEMENTED IN CODEGEN.C", root->type);
            break;
    }
}

void main(int argc, char **argv) {

    return 0;
}