#include <stdio.h>
#include <stdlib.h>


typedef struct cell_t {
  char value;
  char visited;
  int row;
  int col;
  struct cell_t* parent;
} cell_t;

typedef struct {

  cell_t* cells;


  cell_t** queue; 
  int queue_cap;
  int queue_size;
  int queue_front;
  int queue_rear;

  int width;
  int height;

  cell_t* start;
  cell_t* end;



} grid_t;

static int init_grid(grid_t* grid, int grid_width, int grid_height) {
  grid->cells = malloc(sizeof(cell_t) * grid_width * grid_height);
  grid->queue = malloc(sizeof(cell_t*) * grid_width * grid_height);
  grid->queue_size = 0;
  grid->queue_front = 0;
  grid->queue_rear = 0;
  grid->queue_cap = grid_width * grid_height;

  grid->width = grid_width;
  grid->height = grid_height;
  return grid->cells != 0;
}


static void free_grid(grid_t* grid) {
  free(grid->queue);
  free(grid->cells);
}

// Returns the cell to be enqueued
static cell_t** 
enqueue_cell(grid_t* grid) {
  // Full case, return null
  if (grid->queue_size >= grid->queue_cap) return 0;

  cell_t** ret = grid->queue + grid->queue_rear++;
  grid->queue_rear = (grid->queue_rear) % grid->queue_cap;
  grid->queue_size += 1;

  //printf("queue size: %d/%d\n", grid->queue_size, grid->queue_cap);
  return ret;
}


static cell_t**
dequeue_cell(grid_t* grid) {
  // Empty case, return null
  if (grid->queue_size <= 0) return 0;

  cell_t** ret = grid->queue + grid->queue_front++;
  grid->queue_front = (grid->queue_front) % grid->queue_cap;
  grid->queue_size -= 1;

  return ret;
}

static int get_cell_index(grid_t* grid, int r, int c) {
  return c + (r*grid->width);
}

static cell_t* get_grid_cell(grid_t* grid, int r, int c) {
  if (r >= grid->height || c >= grid->width || r < 0 || c < 0) return 0; 
  int index = get_cell_index(grid, r, c);
  return grid->cells + index;
}


static void 
explore_neighbour(grid_t* grid, cell_t* parent, cell_t* cell) {
  if (cell == 0 || cell->visited == 1) return;

  int height_diff = cell->value - parent->value; 
//  if (height_diff < 0) height_diff = -height_diff;

  if(cell->value == 'S' || cell->value == 'E' || 
     parent->value == 'S' || parent->value == 'E' ||
     height_diff <= 1) 
  {

    //printf("queuing   (%p)(%c): %d %d\n", cell, cell->value, cell->row, cell->col);
    cell->visited = 1;
    cell->parent = parent;
    *enqueue_cell(grid) = cell;
  }
}


static void bfs(grid_t* grid) {

  
  // put starting cell into queue
  // we don't care about error checking here; 
  // it will crash expectedly if null is returned.
  grid->start->visited = 1;
  *enqueue_cell(grid) = grid->start;

  // while the queue is not empty
  while(grid->queue_size != 0) {
    // explore cell from front of queue
    cell_t* cur_cell = *dequeue_cell(grid);
    //printf("exploring (%p)(%c): %d %d\n", cur_cell, cur_cell->value, cur_cell->row, cur_cell->col);

    if (cur_cell == grid->end) {
      printf("bfs done!\n");
      return;
      // End the loop here
      //return cur_cell;
    }
    else 
    {

      explore_neighbour(grid, cur_cell, get_grid_cell(grid, cur_cell->row-1, cur_cell->col));
      explore_neighbour(grid, cur_cell, get_grid_cell(grid, cur_cell->row+1, cur_cell->col));
      explore_neighbour(grid, cur_cell, get_grid_cell(grid, cur_cell->row, cur_cell->col-1));
      explore_neighbour(grid, cur_cell, get_grid_cell(grid, cur_cell->row, cur_cell->col+1));
    }

  }
  printf("bfs died\n");

}


static int is_valid_char(char c) {
  if (c >= 'a' && c <= 'z') return 1;
  if (c == 'S' || c == 'E') return 1;
  return 0;
}

int main() { 
  grid_t grid = {0};

  FILE* fp = fopen("input.txt", "r");
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
          cell_t* cell = get_grid_cell(&grid, row, col); 
          if (cell) {
            cell->visited = 0;
            if (buffer[col] == 'S') {
              grid.start = cell;
              cell->value = 'a';
            }
            if (buffer[col] == 'E') {
              grid.end = cell;
              cell->value = 'z';
            }
            else {
              cell->value = buffer[col];
            }
            cell->row = row;
            cell->col = col;
          }
          else {
            printf("Invalid cell: %d %d!\n", row, col);
            return 0;
          }
        }
      }
    
#if 0
      // Test queue

      cell_t ** c = 0;
      c = enqueue_cell(&grid), *c = grid.cells + 0;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 1;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 2;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 3;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 4;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);


      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);

      c = enqueue_cell(&grid), *c = grid.cells + 0;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 1;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = enqueue_cell(&grid), *c = grid.cells + 2;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
#endif




      bfs(&grid);
#if 0
      // print
      for (int r = 0; r < grid.height; ++r) {
        for (int c = 0; c < grid.width; ++c) {
          cell_t *cell = get_grid_cell(&grid,r,c);
          if (cell->parent == 0) printf(".");
          else {
            if (cell->parent->row == cell->row-1) printf("v");
            if (cell->parent->row == cell->row+1) printf("^");
            if (cell->parent->col == cell->col+1) printf("<");
            if (cell->parent->col == cell->col-1) printf(">");
          }
        }
        printf("\n");
      }
#endif
#if 1
      // print path
      {
        cell_t* cell = grid.end;
        int count = 0;
        while(cell != 0) {
          ////printf("%c: %d %d\n", cell->value, cell->row, cell->col);
          cell = cell->parent; 
          count ++;
        }
        printf("%d steps!\n", count-1);

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
