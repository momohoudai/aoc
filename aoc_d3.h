

static u32_t 
d3_get_priority(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 1;
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 27;
  }
  return 0;
}

static void 
d3a(const char* filename) {
  u32_t score = 0;
  FILE* fp = fopen(filename, "r");
  char buffer[128];
  if (fp) {
    u32_t i = 1;
    u32_t priority = 0;
    while(fgets(buffer, sizeof(buffer), fp)) {
      // NOTE(momo): There are no odd cases!
      u32_t len = cstr_len_ext(buffer, is_alpha);      
      u32_t split = len/2;

      // Find first common item 
      char common = 0;
      for (u32_t i = 0; i < split; ++i) 
      {
        for(u32_t j = split; j < len; ++j)
        {
          if (buffer[i] == buffer[j]){
            common = buffer[i];
            goto found;
          }

        }
      }

found:
      priority = d3_get_priority(common); 
      //printf("#%d: common is %c: %d! (%d, %d)\n", i, common, priority, split, len);
      score += priority; 

      ++i;



    }

    //printf("sum: %d\n", sum);
    printf("%d", score);
    fclose(fp);
  }
}

static void 
d3b(const char* filename) 
{
  u32_t score = 0;
  FILE* fp = fopen(filename, "r");
  char buffer[3][128];

  if (fp) {
    u32_t rucksack_index = 0;
    u32_t priority = 0;
    // NOTE(momo): There are exactly 300 lines
    while(1) {
      char common = 0;
      // Fill buffers
      for (u32_t i = 0; i < 3; ++i)
      {
        if(!fgets(buffer[i], sizeof(buffer[i]), fp)) {
          goto finished;
        }
      }

      // search for common shit between all 3 bags
      for (char* it0 = buffer[0]; is_alpha(*it0); ++it0)
      {
        for(char* it1 = buffer[1]; is_alpha(*it1); ++it1)
        {
          for(char* it2 = buffer[2]; is_alpha(*it2); ++it2)
          {
            if (*it0 == *it1 && *it1 == *it2) {
              common = *it0;
              goto found;
            }

          }

        }
      }

found:
      score += d3_get_priority(common); 

    }

finished:

    printf("%d", score);
    fclose(fp);
  }


}
