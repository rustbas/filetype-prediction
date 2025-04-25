#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MSIZE 256

size_t verbose = 0;

typedef unsigned char u8;

typedef struct {
  u8 *data;
  size_t size;
} raw_data;

int read_file(const char *filepath, raw_data *rd) {
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

int build_matrix(raw_data *rd, uint32_t matrix[MSIZE][MSIZE]) {
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

int write_matrix_into_file(const char *filepath, uint32_t matrix[MSIZE][MSIZE]) {
 uint32_t matrix_raw[MSIZE*MSIZE] = {0};

 for (size_t i=0; i<MSIZE; i++)
   for (size_t j=0; j<MSIZE; j++)
     matrix_raw[i*MSIZE + j] = matrix[i][j];

  FILE *file = fopen(filepath, "wb");
  fwrite(matrix_raw, MSIZE*MSIZE*sizeof(uint32_t), 1, file);
  fclose(file);
}

int main(size_t argc, const char **argv) {

  uint32_t matrix[MSIZE][MSIZE] = {0};
  raw_data rd = {0};

  char input[256] = "";
  char output[256] = "";
  
  if (argc < 5) {
    fprintf(stderr, "Usage: convert -i <input binary file> -o <matrix file> [-v <verbose level: 0|1|2>]");
    return 1;
  }

  for (size_t i=1; i<argc; i++) {
    if (strcmp(argv[i], "-i") == 0)
      strcpy(input, argv[++i]);
    if (strcmp(argv[i], "-o") == 0)
      strcpy(output, argv[++i]);
    if (strcmp(argv[i], "-v") == 0)
      verbose = atoi(argv[++i]);
  }
  
  if (verbose)
    printf("[INFO] Computing file: %s\n", input);

  
  read_file(input, &rd);
  build_matrix(&rd, matrix);
  write_matrix_into_file(output, matrix);

  
  free(rd.data);
  return 0;
}
