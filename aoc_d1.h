// https://adventofcode.com/2022/day/1



static u32_t 
d1a(const char* filename) 
{
  u32_t max = 0;
  u32_t sum = 0;
  FILE* fp = fopen(filename, "r");
  char buffer[64];
  if (fp) {
    while(fgets(buffer, array_count(buffer), fp)) {
      if (buffer[0] == '\n' || buffer[0] == '\r') {
        //printf("sum of elf: %d\n", sum); 
        if (sum > max) {
          //printf("MAX DETECTED!\n");
          max = sum;
        }
        sum = 0;
      }
      else {
        sum += cstr_to_u32(buffer);
        //printf("sum is now: %d\n", sum); 
      }
    }

    //printf("fattest elf: %d\n", max);
    fclose(fp);
  }
  return max;
}
