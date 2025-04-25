#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MSIZE 256

size_t verbose = 1;

typedef unsigned char u8;

typedef struct {
  u8 *data;
  size_t size;
} raw_data;

int main(size_t argc, const char **argv) {

  uint32_t matrix[MSIZE][MSIZE] = {0};
  
  if (argc == 1 || argc > 2) {
    fprintf(stderr, "Usage: convert <input binary file>");
    return 1;
  }
  
  if (verbose)
    printf("[INFO] Computing file: %s\n", argv[1]);

  raw_data rd = {};
  size_t idx1, idx2;
  
  FILE *file = fopen(argv[1], "rb");

  fseek(file, 0, SEEK_END); // Jump to the EOF
  rd.size = ftell(file);   // Get offset of file
  rewind(file);             // Return to begin of file

  rd.data = (u8*) malloc(rd.size * sizeof(u8));
  fread(rd.data, rd.size, 1, file);  

  if (verbose)
    printf("[INFO] Readed %zu bytes\n", rd.size);

  for (size_t i=0; i<rd.size-1; i++){
    idx1 = rd.data[i];
    idx2 = rd.data[i+1];
    matrix[idx1][idx2]++;
  }

  for (size_t i=0; i<4; i++)
    printf("%08X ", matrix[0][i]);
  printf("\n");
  
  free(rd.data);
  fclose(file);
  return 0;
}
