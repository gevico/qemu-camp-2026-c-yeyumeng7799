// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *start = str;
    while (*start && isspace(*start)) start++;
    char *end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) end--;
    *(end + 1) = '\0';
    if (start != str) memmove(str, start, strlen(start) + 1);
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  *dict_count = 0;

  while (fgets(line, sizeof(line), file)) {
      char *sep = strchr(line, '\t');
      if (!sep) sep = strchr(line, ' ');
      if (!sep) continue;
      
      *sep = '\0';
      char *word = line;
      char *trans = sep + 1;
      
      trim(word);
      trim(trans);
      
      if (strlen(word) > 0 && strlen(trans) > 0) {
          hash_table_insert(table, word, trans);
          (*dict_count)++;
      }
  }

  fclose(file);
  return 0;
}
