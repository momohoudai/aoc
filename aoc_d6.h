
static void 
d6a(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    char* buffer = malloc(file_size);
    if( buffer ) {
      fgets(buffer, file_size, fp);
      int beg = 0, end = 3;
      int repeat_found = 1;
      
      while(repeat_found && end < file_size) {
        //printf("beg vs end: %d vs %d\n", beg, end);
        repeat_found = 0;

        for(int it1 = beg; it1 <= end-1; ++it1) {
          for (int it2 = it1+1; it2 <= end; ++it2) {
            //printf("checking %c vs %c\n", buffer[it1], buffer[it2]);
            if (buffer[it1] == buffer[it2]) {
              repeat_found = 1;
              goto escape;

            }
              


          }
        }

escape:
        ++beg;
        ++end;

      }


      printf("%d", end);

      free(buffer);
    }



    fclose(fp);
  }

}

static void
d6b(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    char* buffer = malloc(file_size);
    if( buffer ) {
      fgets(buffer, file_size, fp);
      int beg = 0, end = 13;
      int repeat_found = 1;
      
      while(repeat_found && end < file_size) {
        //printf("beg vs end: %d vs %d\n", beg, end);
        repeat_found = 0;

        for(int it1 = beg; it1 <= end-1; ++it1) {
          for (int it2 = it1+1; it2 <= end; ++it2) {
            //printf("checking %c vs %c\n", buffer[it1], buffer[it2]);
            if (buffer[it1] == buffer[it2]) {
              repeat_found = 1;
              goto escape;

            }
              


          }
        }

escape:
        ++beg;
        ++end;

      }


      printf("%d", end);

      free(buffer);
    }



    fclose(fp);
  }

}

 
