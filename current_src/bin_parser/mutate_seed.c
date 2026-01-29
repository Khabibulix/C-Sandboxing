#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



#define HEADER_SIZE 		12
#define MAX_MUTATIONS 		6
#define MAX_INSERT_SIZE		16
#define MAX_FILE_SIZE		(1024 * 1024)


static int rand_range(int min, int max){
  return min + (rand() % (max - min + 1));
}

void mutate(uint8_t **buffer, size_t *len){
  int mutations;
  size_t i;

  if (!buffer || !*buffer || !len) return;
  if (*len <= HEADER_SIZE) return;

  mutations = rand_range(1, MAX_MUTATIONS);

  for (i = 0; i < (size_t)mutations; i++){
    int choice = rand_range(0, 3);

    // BIT FLIP
    if (choice == 0) {
      size_t pos = rand_range(HEADER_SIZE, *len - 1);
      uint8_t bit = 1 << rand_range(0, 7);
      (*buffer)[pos] ^= bit;
    }

    // BYTE OVERWRITE
    else if (choice == 1){
      size_t pos = rand_range(HEADER_SIZE, *len - 1);
      (*buffer)[pos] = (uint8_t)rand();
    }

    // INSERT BYTES
    else if (choice == 2){
      size_t pos;
      size_t ins_len;
      uint8_t *new_buf;

      if (*len >= MAX_FILE_SIZE) continue;

      pos = rand_range(HEADER_SIZE, *len);
      ins_len = rand_range(1, MAX_INSERT_SIZE);

      if (*len + ins_len > MAX_FILE_SIZE) ins_len = MAX_FILE_SIZE - *len;
      
      new_buf = malloc(*len + ins_len);
      if (!new_buf) continue;

      memcpy(new_buf, *buffer, pos);

      for (size_t j = 0; j < ins_len; j++){
        new_buf[pos + j] = (uint8_t)rand();
      }

      memcpy(new_buf + pos + ins_len, *buffer + pos, *len - pos);

      free(*buffer);
      *buffer = new_buf;
      *len += ins_len;

    }

    // DELETE BYTES
    else if (choice == 3){
      size_t pos;
      size_t del_len;
      uint8_t *new_buf;

      if (*len <= HEADER_SIZE + 1) continue;

      pos = rand_range(HEADER_SIZE, *len - 1);
      del_len = rand_range(1, MAX_INSERT_SIZE);
      
      if (pos + del_len > *len) del_len = *len - pos;

      new_buf = malloc(*len - del_len);
      if (!new_buf) continue;

      memcpy(new_buf, *buffer, pos);
      memcpy(new_buf + pos, *buffer + pos + del_len, *len - pos - del_len);
      
      free(*buffer);
      *buffer = new_buf;
      *len -= del_len;
 
    }

  }


}


char *build_filename(const char *dir, int index){
  const char *prefix = "mut_";
  const char *suffix = ".bin";
  char index_buf[16];
  size_t len;
  char *path;
  int need_slash;

  if (!dir || index < 0) return NULL;
  
  snprintf(index_buf, sizeof(index_buf), "%06d", index);
  need_slash = (dir[strlen(dir) -1] != '/');

  len = strlen(dir) + (need_slash ? 1 : 0) + strlen(prefix) + strlen(index_buf) + strlen(suffix) + 1;

  path = malloc(len);
  if (!path) return NULL;

  strcpy(path, dir);
  if (need_slash) strcat(path, "/");

  strcat(path, prefix);
  strcat(path, index_buf);
  strcat(path, suffix);

  return path;

}


int write_file(const char *path, const uint8_t *buffer, size_t len){
  FILE *f;
  if (!path || !buffer || len == 0) return -1;

  f = fopen(path, "wb");
  if (!f) return -1;

  if (fwrite(buffer, 1, len, f) != len) {
    fclose(f);
    return -1;
  }

  fclose(f);
  return 0;
}

int load_file(const char *path, uint8_t **buffer, size_t *len){
  FILE *f = fopen(path, "rb");
  if (!f) return -1;

  long filesize;
  uint8_t *tmp = NULL;

  if (!path || !buffer || !len) return -1;
  if (fseek(f, 0, SEEK_END) != 0) goto error;

  filesize = ftell(f);
  if (filesize <= 0) goto error;
  if (fseek(f, 0, SEEK_SET) != 0) goto error;

  tmp = malloc((size_t)filesize);
  if (!tmp) goto error;

  if (fread(tmp, 1, (size_t)filesize, f) != (size_t)filesize) goto error;

  fclose(f);

  *buffer = tmp;
  *len = (size_t)filesize;
  return 0;

error:
  if (f)
    fclose(f);
  free(tmp);
  return -1;

}

static void usage(const char *prog_name){
  fprintf(stderr, "Usage: %s <seed_file> <output_dir> <count> \n", prog_name);
}



// ./mutate.exe <seed_file> <output_dir> <count>
int main(int argc, char **argv){
  const char *seed_path;
  const char *output_dir;
  int count;
  unsigned int rng_seed;
  uint8_t *seed_buffer = NULL;
  size_t seed_len = 0;

  if (argc < 4 || argc > 5){
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  seed_path = argv[1];
  output_dir = argv[2];
  count = atoi(argv[3]);

  if (count <= 0){
    fprintf(stderr, "[-] Invalid count value\n");
    return EXIT_FAILURE;
  }

  if (load_file(seed_path, &seed_buffer, &seed_len) != 0){
    fprintf(stderr, "[-] Failed to load seed file: %s\n", seed_path);
    return EXIT_FAILURE;
  }

  if (seed_len == 0){
    fprintf(stderr, "[-] Seed file is empty\n");
    free(seed_buffer);
    return EXIT_FAILURE;
  }

  for (int i = 0; i < count; i++){
    uint8_t *mutant_buffer = NULL;
    size_t mutant_len = seed_len;
    mutant_buffer = malloc(seed_len);
    
    if (!mutant_buffer){
      fprintf(stderr, "[-] malloc failed\n");
      break;
    }

    memcpy(mutant_buffer, seed_buffer, seed_len);
    mutate(&mutant_buffer, &mutant_len);

    char *out_path = build_filename(output_dir, i);
    if (!out_path){
      fprintf(stderr, "[-] Failed to build output filename\n");
      free(mutant_buffer);
      break;
    }

    if (write_file(out_path, mutant_buffer, mutant_len) != 0){
      fprintf(stderr, "[-] Failed to write %s\n", out_path);
    }

    free(out_path);
    free(mutant_buffer);
  }

  free(seed_buffer);
  return EXIT_SUCCESS;

}
