/*
section     .text
global      _start

_start:
    mov     edx,len   
    mov     ecx,msg   
    mov     ebx,1   
    mov     eax,4   
    int     0x80   
    mov     eax,1  
    int     0x80   

section     .data
msg     db  'Hello world',0xa  
len     equ $ - msg
*/

#include <stdio.h>

void generate_print(char* value) {
  printf("section\t.text\n");
  printf("global\t_start\n");

  printf("_start:\n");
  printf("\tmov\tedx, len\n");
  printf("\tmov\tecx, msg\n");
  printf("\tmov\tebx, 1\n");
  printf("\tmov\teax, 4\n");
  printf("\tint\t0x80\n");
  printf("\tmov\teax, 1\n");
  printf("\tint\t0x80\n");

  printf("section\t.data\n");
  printf("msg\tdb\t'%s', 0xa\n", value);
  printf("len\tequ $ - msg\n");
}

void generate_assembly() {
  generate_print("Hello World");
}

int main(int argc, char** argv) {
  generate_assembly();
  return 0;
}