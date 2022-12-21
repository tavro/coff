#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intermediate_code {
  int opcode;
  char* operand1;
  int operand2;
  int result;
};

void generate_code(struct intermediate_code* code) {
  switch (code->opcode) {
    case 1:
      printf("CONCAT R%d, %s, #%d\n", code->result, code->operand1, code->operand2);
      break;
    case 2:
      printf("MOV R%d, #%d\n", code->result, code->operand2);
      break;
    case 3:
      printf("LOAD R%d, %s\n", code->result, code->operand1);
      break;
    default:
      fprintf(stderr, "Error: Unknown opcode %d\n", code->opcode);
      break;
  }
}

int main(int argc, char** argv) {
  struct intermediate_code code1;
  code1.opcode = 1;
  code1.operand1 = "hello";
  code1.operand2 = 3;
  code1.result = 4;
  generate_code(&code1);

  struct intermediate_code code2;
  code2.opcode = 2;
  code2.operand1 = NULL;
  code2.operand2 = 5;
  code2.result = 6;
  generate_code(&code2);

  struct intermediate_code code3;
  code3.opcode = 3;
  code3.operand1 = "x";
  code3.operand2 = 0;
  code3.result = 7;
  generate_code(&code3);

  return 0;
}

