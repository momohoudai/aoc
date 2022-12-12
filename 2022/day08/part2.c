#include <stdio.h>
#include <stdlib.h>

typedef struct grid_t {
  int height;
  int score; // for debugging
}grid_t;

static int is_whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\r';
}

int main() {
  FILE* fp = fopen("input.txt", "r");
  if (fp) {
    char buffer[128];

    // determine grid width and height
    int grid_width = 0;
    int grid_height = 0;

    while (fgets(buffer, sizeof(buffer), fp))
    {
      if (grid_width == 0) {
        for(char* it = buffer; !is_whitespace(*it); ++it)
        {
          ++grid_width;
        }
      }

      ++grid_height;
    }

    fseek(fp, 0, SEEK_SET); 
   
    

    grid_t* grid = malloc(grid_width * grid_height * sizeof(grid_t));
#define cell(x,y)  grid[(x) + (grid_width)*(y)]

    if (grid) {
      // initialize the grid
      int y = 0;
      while (fgets(buffer, sizeof(buffer), fp))
      {
        int x = 0;
        for(char* it = buffer; !is_whitespace(*it); ++it)
        {
          cell(x,y).height = (*it)-'0';
          ++x;
        }
        ++y;
      }

#if 1
      // debug print
      for (int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width; ++j) {
          printf("%d,", cell(j,i).height);
        }
        printf("\n");
      }
      printf("\n");
#endif

      int highest_score = 0;
      int hx, hy, hl, hb, ht, hr;

      // For every damn tree, we have to beam up down left and right
      for (int y = 0; y < grid_height; ++y) {
        for (int x = 0; x < grid_width; ++x) {
          int tree_height = cell(x,y).height;
          // BEAM LEFT
          int left_score = 0;
          for(int l = x-1; l >= 0; --l) {
            int cur_tree_height = cell(l,y).height;
            ++left_score;
            if (cur_tree_height >= tree_height) {
              break;
            }
            
          }
          

          // BEAM RIGHT
          int right_score = 0;
          for(int r = x+1; r < grid_width; ++r) {
            int cur_tree_height = cell(r,y).height;
            ++right_score;
            if (cur_tree_height >= tree_height) {
              break;
            }
            
          }
          
          int top_score = 0;
          for(int t = y-1; t >= 0; --t) {
            int cur_tree_height = cell(x,t).height;
            ++top_score;
            if (cur_tree_height >= tree_height) {
              break;
            }
            

          }
          
          int bottom_score = 0;
          for(int b = y+1; b < grid_height; ++b) {
            int cur_tree_height = cell(x,b).height;
            ++bottom_score;
            if (cur_tree_height >= tree_height) {
              break;
            }
            

          }

          //printf("%d, %d: %d %d %d %d\n", x, y, left_score, right_score, top_score, bottom_score);
          int score = left_score * right_score * top_score * bottom_score;
          cell(x,y).score = score; 
          if (highest_score < score) {
            highest_score = score;
            hx = x; 
            hy = y;
            hl = left_score;
            hr = right_score;
            ht = top_score;
            hb = bottom_score;
          }


        }
      }

#if 0 
      // debug print
      for (int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width; ++j) {
          printf("%d,", cell(j,i).score);
        }
        printf("\n");
      }
#endif
      printf("Best Score: %d\n at (%d, %d) with (%d %d %d %d)\n", highest_score, hx, hy, hl, hr, ht, hb);

      free(grid);
    }




    fclose(fp);
  }

}
