#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MSIZE 256

typedef unsigned char u8;

typedef struct {
  u8 *data;
  size_t size;
} raw_data;

int read_file(const char *filepath, raw_data *rd, ssize_t verbose) {
  FILE *file = fopen(filepath, "rb");

  fseek(file, 0, SEEK_END); // Jump to the EOF
  rd->size = ftell(file);   // Get offset of file
  rewind(file);             // Return to begin of file

  rd->data = (u8*) malloc(rd->size * sizeof(u8));
  fread(rd->data, rd->size, 1, file);
  
  if (verbose == 2)
    printf("[INFO] Readed %zu bytes\n", rd->size);

  fclose(file);
  return 0;
}

int build_matrix(raw_data *rd, uint32_t matrix[MSIZE][MSIZE], ssize_t verbose) {
  size_t idx1, idx2;
  for (size_t i=0; i<rd->size-1; i++){
    idx1 = rd->data[i];
    idx2 = rd->data[i+1];
    matrix[idx1][idx2]++;
  }
  
  if (verbose == 2) {
    printf("[INFO] First 4 elements\n    ");
    for (size_t i=0; i<4; i++)
      printf("%08X ", matrix[0][i]);
    printf("\n");
  }
  return 0;
}
