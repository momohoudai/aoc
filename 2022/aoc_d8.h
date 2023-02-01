typedef struct {
  int height;
  int score; // for debugging
} d8_grid_t;



static void
d8a(const char* filename) {
  FILE* fp = fopen(filename, "r");
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

    // printf("grid dims: %d %d\n", grid_width, grid_height);
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

#if 0
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
      printf("%d", count);

      free(grid);
    }




    fclose(fp);
  }

}


static void
d8b(const char* filename) {
  FILE* fp = fopen(filename, "r");
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
   
    

    d8_grid_t* grid = malloc(grid_width * grid_height * sizeof(d8_grid_t));
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

#if 0
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
      printf("%d", highest_score);

      free(grid);
    }




    fclose(fp);
  }

}
