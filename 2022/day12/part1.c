#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char* cells;
  int width;
  int height;
} grid_t;

static int init_grid(grid_t* grid, int grid_width, int grid_height) {
  grid->cells = malloc(sizeof(char) * grid_width * grid_height);
  grid->width = grid_width;
  grid->height = grid_height;
  return grid->cells != 0;
}

static void free_grid(grid_t* grid) {
  free(grid->cells);
}

static char* get_grid_cell(grid_t* grid, int r, int c) {
  if (r >= grid->height || c >= grid->width) return 0; 
  return grid->cells + c + (r*grid->width);
}

static int is_valid_char(char c) {
  if (c >= 'a' && c <= 'z') return 1;
  if (c == 'S' || c == 'E') return 1;
  return 0;
}

int main() { 
  grid_t grid = {0};

  FILE* fp = fopen("test.txt", "r");
  if (fp) 
  {
    // one pass to check width and height
    int grid_width = 0;
    int grid_height = 0;
    {
      char buffer [256];
      int first_time = 1;
      while(fgets(buffer, 256, fp))
      {
        if (first_time) 
        {
          for(char* it = buffer; 
              is_valid_char(it[0]);
              ++it ) 
          {
            ++grid_width;
          }
          first_time = 0;
        }
        ++grid_height;
      }
      printf("grid dims: %d %d\n", grid_width, grid_height);
    }
    fseek(fp, 0, SEEK_SET);

    if (init_grid(&grid, grid_width, grid_height)) {
      char buffer [256];
      for(int row = 0; row < grid.height; ++row)
      {
        fgets(buffer, 256, fp);
        for(int col = 0; col < grid.width; ++col) 
        {
          char* cell = get_grid_cell(&grid, row, col); 
          if (cell) {
            *cell = buffer[col];
          }
          else {
            printf("Invalid cell: %d %d!\n", row, col);
            return 0;
          }
        }
      }
#if 1
      // print
      for (int r = 0; r < grid.height; ++r) {
        for (int c = 0; c < grid.width; ++c) {
          printf("%c", *get_grid_cell(&grid,r,c));
        }
        printf("\n");
      }
#endif
      free_grid(&grid);
    }
    else {
      printf("Cannot init grid\n");
    }

    



    fclose(fp);

  }
}
