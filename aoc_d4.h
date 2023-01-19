// https://adventofcode.com/2022/day/4

static u32_t
d4a(const char* filename) {

  u32_t overlapping_pairs =  0;; 
  FILE* fp = fopen(filename, "r");
  char buffer[16];
  if (fp) {

    while(fgets(buffer, sizeof(buffer), fp)) {
      tokenizer_t tokenizer = { buffer, 0 };
      r2u_t first_elf;
      r2u_t second_elf;

      first_elf.min = tokenizer_get_next_u32(&tokenizer);
      first_elf.max = tokenizer_get_next_u32(&tokenizer);

      second_elf.min = tokenizer_get_next_u32(&tokenizer);
      second_elf.max = tokenizer_get_next_u32(&tokenizer);

      u32_t overlap = r2u_is_fully_overlapping(first_elf, second_elf);
#if 0
      printf("%d-%d,%d-%d: %d\n", 
             first_elf.min, first_elf.max, 
             second_elf.min, second_elf.max, 
             overlap);
#endif

      overlapping_pairs += overlap;


    }
    //printf("Overlapping pairs: %d\n", overlapping_pairs);
    fclose(fp);
  }

  return overlapping_pairs;
}

static u32_t
d4b(const char* filename) {

  u32_t overlapping_pairs =  0;; 
  FILE* fp = fopen(filename, "r");
  char buffer[16];
  if (fp) {
    while(fgets(buffer, sizeof(buffer), fp)) {
      tokenizer_t tokenizer = { buffer, 0 };
      r2u_t first_elf;
      r2u_t second_elf;

      first_elf.min = tokenizer_get_next_u32(&tokenizer);
      first_elf.max = tokenizer_get_next_u32(&tokenizer);

      second_elf.min = tokenizer_get_next_u32(&tokenizer);
      second_elf.max = tokenizer_get_next_u32(&tokenizer);

      u32_t overlap = r2u_is_overlapping(first_elf, second_elf); 
#if 0
      printf("%d-%d,%d-%d: %d\n", 
             first_elf.min, first_elf.max, 
             second_elf.min, second_elf.max, 
             overlap);
#endif

      overlapping_pairs += overlap;


    }

    //printf("Overlapping pairs: %d\n", overlapping_pairs);
    fclose(fp);
  }

  return overlapping_pairs;


}
