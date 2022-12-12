#include <stdio.h>
#include <stdlib.h>


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

    printf("grid dims: %d %d\n", grid_width, grid_height);
    fseek(fp, 0, SEEK_SET); 
   
    

    char* grid = malloc(grid_width * grid_height);
#define VISIBLE_FLAG 1 << 4;
#define cell(x,y)  grid[(x) + (grid_width)*(y)]
#define cell_set_visible(x,y) cell(x,y) |= VISIBLE_FLAG 
#define cell_get_value(x,y) (cell(x,y) & 15)
#define cell_get_visible(x,y) cell(x,y) >> 4
    if (grid) {
      // initialize the grid
      int y = 0;
      while (fgets(buffer, sizeof(buffer), fp))
      {
        int x = 0;
        for(char* it = buffer; !is_whitespace(*it); ++it)
        {
          cell(x,y) = (*it)-'0';
          ++x;
        }
        ++y;
      }

#if 1
      // debug print
      for (int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width; ++j) {
          printf("%d", cell_get_value(j,i));
        }
        printf("\n");
      }
      printf("\n");
#endif


      // For each row, beam left and right side
      // Flag traversed trees 
      for(int y = 0; y < grid_height; ++y) {
        // left beam
        for (int x = 0, last_tree = -1; x < grid_width; ++x) {
          int cur_tree = cell_get_value(x,y);
          if (cur_tree > last_tree) {
            cell_set_visible(x,y);
            last_tree = cur_tree;
          }
          
        }
       
        //right beam
        for (int x = grid_width-1, last_tree = -1; x >= 0; --x) {
          int cur_tree = cell_get_value(x,y);
          if (cur_tree > last_tree) {
            cell_set_visible(x,y);
            last_tree = cur_tree;
          }
          
        }
       
      }
    
      // For each row, beam top and bottom side
      for(int y = 0; y < grid_width; ++y) {
        // top beam
        for (int x = 0, last_tree = -1; x < grid_height; ++x) {
          int cur_tree = cell_get_value(y,x);
          if (cur_tree > last_tree) {
            cell_set_visible(y,x);
            last_tree = cur_tree;
          }
        }
       
        //btm beam
        for (int x = grid_height-1, last_tree = -1; x >= 0; --x) {
          int cur_tree = cell_get_value(y,x);
          if (cur_tree > last_tree) {
            cell_set_visible(y,x);
            last_tree = cur_tree;
          }
          
        }
      }

#if 0
      // debug print
      for (int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width; ++j) {
          printf("%d", cell_get_visible(j,i));
        }
        printf("\n");
      }
#endif
      // debug print
      int count = 0;
      for (int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width; ++j) {
          count += cell_get_visible(j,i);
        }
      }
      printf("visible trees: %d\n", count);

      free(grid);
    }




    fclose(fp);
  }

}
