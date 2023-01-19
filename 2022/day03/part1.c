#include <stdio.h>
#include <string.h>
// https://adventofcode.com/2022/day/3

int cstr_len(const char* str ) {
  // common strlen that counts what I care
  int ret = 0;
  while(*str >= 'a' && *str <= 'z' ||
        *str >= 'A' && *str <= 'Z') 
  {
    ++str;
    ++ret;
  }

  return ret;
  
}

int get_priority(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 1;
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 27;
  }
  return 0;
}


int main() {

  int score = 0;
  FILE* fp = fopen("input.txt", "r");
  char buffer[128];
  if (fp) {
    int sum = 0;
    int i = 1;
    int priority = 0;
    while(fgets(buffer, sizeof(buffer), fp)) {
      // NOTE(momo): There are no odd cases!
      int len = str_len(buffer);      
      int split = len/2;

      // Find first common item 
      char common = 0;
      for (int i = 0; i < split; ++i) 
      {
        for(int j = split; j < len; ++j)
        {
          if (buffer[i] == buffer[j]){
            common = buffer[i];
            goto found;
          }

        }
      }

found:
      priority = get_priority(common); 
      printf("#%d: common is %c: %d! (%d, %d)\n", i, common, priority, split, len);
      sum += priority; 

      ++i;



    }

    printf("sum: %d\n", sum);


    fclose(fp);
  }


}

