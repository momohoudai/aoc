
typedef struct d12_cell_t {
  char value;
  char visited;
  s32_t row;
  s32_t col;
  struct d12_cell_t* parent;
} d12_cell_t;

typedef struct {
  d12_cell_t* cells;
  d12_cell_t** queue; 
  s32_t queue_cap;
  s32_t queue_size;
  s32_t queue_front;
  s32_t queue_rear;

  s32_t width;
  s32_t height;

  d12_cell_t* start;
  d12_cell_t* end;

} d12_grid_t;

static s32_t d12_init_grid(d12_grid_t* grid, s32_t grid_width, s32_t grid_height) {
  grid->cells = malloc(sizeof(d12_cell_t) * grid_width * grid_height);
  grid->queue = malloc(sizeof(d12_cell_t*) * grid_width * grid_height);
  grid->queue_size = 0;
  grid->queue_front = 0;
  grid->queue_rear = 0;
  grid->queue_cap = grid_width * grid_height;

  grid->width = grid_width;
  grid->height = grid_height;
  return grid->cells != 0;
}


static void d12_free_grid(d12_grid_t* grid) {
  free(grid->queue);
  free(grid->cells);
}

// Returns the cell to be enqueued
static d12_cell_t** 
d12_enqueue_cell(d12_grid_t* grid) {
  // Full case, return null
  if (grid->queue_size >= grid->queue_cap) return 0;

  d12_cell_t** ret = grid->queue + grid->queue_rear++;
  grid->queue_rear = (grid->queue_rear) % grid->queue_cap;
  grid->queue_size += 1;

  //printf("queue size: %d/%d\n", grid->queue_size, grid->queue_cap);
  return ret;
}


static d12_cell_t**
d12_dequeue_cell(d12_grid_t* grid) {
  // Empty case, return null
  if (grid->queue_size <= 0) return 0;

  d12_cell_t** ret = grid->queue + grid->queue_front++;
  grid->queue_front = (grid->queue_front) % grid->queue_cap;
  grid->queue_size -= 1;

  return ret;
}

static s32_t 
get_cell_index(d12_grid_t* grid, s32_t r, s32_t c) {
  return c + (r*grid->width);
}

static d12_cell_t* 
d12_get_grid_cell(d12_grid_t* grid, s32_t r, s32_t c) {
  if (r >= grid->height || c >= grid->width || r < 0 || c < 0) return 0; 
  s32_t index = get_cell_index(grid, r, c);
  return grid->cells + index;
}


static void 
d12_explore_neighbour(d12_grid_t* grid, d12_cell_t* parent, d12_cell_t* cell) {
  if (cell == 0 || cell->visited == 1) return;

  s32_t height_diff = cell->value - parent->value; 
//  if (height_diff < 0) height_diff = -height_diff;

  if(cell->value == 'S' || cell->value == 'E' || 
     parent->value == 'S' || parent->value == 'E' ||
     height_diff <= 1) 
  {

    //printf("queuing   (%p)(%c): %d %d\n", cell, cell->value, cell->row, cell->col);
    cell->visited = 1;
    cell->parent = parent;
    *d12_enqueue_cell(grid) = cell;
  }
}


static void 
d12a_bfs(d12_grid_t* grid) {

  
  // put starting cell into queue
  // we don't care about error checking here; 
  // it will crash expectedly if null is returned.
  grid->start->visited = 1;
  *d12_enqueue_cell(grid) = grid->start;

  // while the queue is not empty
  while(grid->queue_size != 0) {
    // explore cell from front of queue
    d12_cell_t* cur_cell = *d12_dequeue_cell(grid);
    //printf("exploring (%p)(%c): %d %d\n", cur_cell, cur_cell->value, cur_cell->row, cur_cell->col);

    if (cur_cell == grid->end) {
      printf("d12_bfs done!\n");
      return;
      // End the loop here
      //return cur_cell;
    }
    else 
    {

      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row-1, cur_cell->col));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row+1, cur_cell->col));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row, cur_cell->col-1));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row, cur_cell->col+1));
    }

  }
  printf("d12_bfs died\n");

}
static s32_t 
d12_prepare_pathfinding(d12_grid_t* grid) {
  for (s32_t i = 0; i < grid->width * grid->height; ++i) {
    grid->cells[i].visited = 0;
    grid->cells[i].parent = 0;
  }
  grid->queue_front = 0;
  grid->queue_rear = 0;
  grid->queue_size = 0;

}
static s32_t 
d12b_bfs(d12_grid_t* grid, d12_cell_t* start_cell) {
  // reset statuses
  
  d12_prepare_pathfinding(grid);

 
 
  // put starting cell into queue
  // we don't care about error checking here; 
  // it will crash expectedly if null is returned.
  start_cell->visited = 1;
  *d12_enqueue_cell(grid) = start_cell;

  // while the queue is not empty
  while(grid->queue_size != 0) {
    // explore cell from front of queue
    d12_cell_t* cur_cell = *d12_dequeue_cell(grid);
    //printf("exploring (%p)(%c): %d %d\n", cur_cell, cur_cell->value, cur_cell->row, cur_cell->col);

    if (cur_cell == grid->end) {
      // count path
      d12_cell_t* itr = grid->end;
      s32_t count = 0;
      while(itr != 0) {
        itr = itr->parent; 
        count++;
      }
      return count;
    }
    else 
    {
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row-1, cur_cell->col));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row+1, cur_cell->col));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row, cur_cell->col-1));
      d12_explore_neighbour(grid, cur_cell, d12_get_grid_cell(grid, cur_cell->row, cur_cell->col+1));
    }

  }

  return 0;

}
static s32_t 
d12_is_valid_char(char c) {
  if (c >= 'a' && c <= 'z') return 1;
  if (c == 'S' || c == 'E') return 1;
  return 0;
}

static void
d12a(const char* filename) { 
  d12_grid_t grid = {0};

  FILE* fp = fopen(filename, "r");
  if (fp) 
  {
    // one pass to check width and height
    s32_t grid_width = 0;
    s32_t grid_height = 0;
    {
      char buffer [256];
      s32_t first_time = 1;
      while(fgets(buffer, 256, fp))
      {
        if (first_time) 
        {
          for(char* it = buffer; 
              d12_is_valid_char(it[0]);
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

    if (d12_init_grid(&grid, grid_width, grid_height)) {
      char buffer [256];
      for(s32_t row = 0; row < grid.height; ++row)
      {
        fgets(buffer, 256, fp);
        for(s32_t col = 0; col < grid.width; ++col) 
        {
          d12_cell_t* cell = d12_get_grid_cell(&grid, row, col); 
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
            return;
          }
        }
      }
    
#if 0
      // Test queue

      d12_cell_t ** c = 0;
      c = d12_enqueue_cell(&grid), *c = grid.cells + 0;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 1;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 2;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 3;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 4;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);


      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);

      c = d12_enqueue_cell(&grid), *c = grid.cells + 0;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 1;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_enqueue_cell(&grid), *c = grid.cells + 2;
      printf("queue: %d %d\n", grid.queue_front, grid.queue_rear);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
      c = d12_dequeue_cell(&grid);
      printf("%c\n", (*c)->value);
#endif




      d12a_bfs(&grid);
#if 0
      // print
      for (s32_t r = 0; r < grid.height; ++r) {
        for (s32_t c = 0; c < grid.width; ++c) {
          d12_cell_t *cell = d12_get_grid_cell(&grid,r,c);
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
        d12_cell_t* cell = grid.end;
        s32_t count = 0;
        while(cell != 0) {
          ////printf("%c: %d %d\n", cell->value, cell->row, cell->col);
          cell = cell->parent; 
          count ++;
        }
        printf("%d steps!\n", count-1);

      }
#endif

      d12_free_grid(&grid);
    }
    else {
      printf("Cannot init grid\n");
    }

    



    fclose(fp);

  }
}

static void
d12b(const char* filename) { 
  d12_grid_t grid = {0};

  FILE* fp = fopen(filename, "r");
  if (fp) 
  {
    // one pass to check width and height
    s32_t grid_width = 0;
    s32_t grid_height = 0;
    {
      char buffer [256];
      s32_t first_time = 1;
      while(fgets(buffer, 256, fp))
      {
        if (first_time) 
        {
          for(char* it = buffer; 
              d12_is_valid_char(it[0]);
              ++it ) 
          {
            ++grid_width;
          }
          first_time = 0;
        }
        ++grid_height;
      }
      //printf("grid dims: %d %d\n", grid_width, grid_height);
    }
    fseek(fp, 0, SEEK_SET);

    if (d12_init_grid(&grid, grid_width, grid_height)) {
      char buffer [256];
      for(s32_t row = 0; row < grid.height; ++row)
      {
        fgets(buffer, 256, fp);
        for(s32_t col = 0; col < grid.width; ++col) 
        {
          d12_cell_t* cell = d12_get_grid_cell(&grid, row, col); 
          if (cell) {
            cell->visited = 0;
            if (buffer[col] == 'S') {
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
            return;
          }
        }
      }
    


#if 0
      // print explored paths and their parents
      for (s32_t r = 0; r < grid.height; ++r) {
        for (s32_t c = 0; c < grid.width; ++c) {
          cell_t *cell = d12_get_grid_cell(&grid,r,c);
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
      s32_t shortest_steps = grid.width * grid.height;
      for (s32_t i = 0; i < grid.width * grid.height; ++i) 
      {
        d12_cell_t* cell = grid.cells + i;
        if (cell->value == 'a') 
        {
          s32_t steps = d12b_bfs(&grid, cell);
          if (steps != 0) 
          {
            printf("steps found: %d\n", steps);
            if (shortest_steps > steps) 
            {
              shortest_steps = steps;
            }
          }
        }
      } 
      printf("shortest steps: %d\n", shortest_steps-1);
      

      d12_free_grid(&grid);
    }
    else {
      printf("Cannot init grid\n");
    }

    



    fclose(fp);

  }
}



