#include <stdio.h>
#include <stdlib.h>

int str_to_int(const char* str) {
  int ret = 0;
  while(*str >= '0' && *str <= '9') {
    ret *= 10;
    ret += (*str - '0'); 
    ++str; 
  }


  return ret;

}

#define abs_of(x) ((x) < 0) ? -(x) : (x)

typedef struct {
  int x;
  int y;
} v2s_t;

typedef struct {
  v2s_t* e;
  int count;
  int cap;
} list_t;

static int 
already_exists(list_t* list, v2s_t pos) {
  for (int i = 0; i < list->count; ++i) {
    if (list->e[i].x == pos.x && list->e[i].y == pos.y) {
      return 1;
    }
  }
  return 0;
}


static int 
insert_unique(list_t* list, v2s_t pos) {
  if (!already_exists(list, pos)) {
    if (list->count >= list->cap) {

      list->cap *= 2;

      void* mem = realloc(list->e, sizeof(v2s_t)*list->cap);
      if (!mem) {
        printf("Realloc failed\n");
        return 0;
      }
      list->e = mem; 

    }

    list->e[list->count++] = pos;
  }

  return 1;
}

static int 
init_list(list_t* list) {
  list->cap = 1024;
  list->count = 0;
  list->e = malloc(sizeof(v2s_t)*1024);

  if (!list->e) return 0;
  return 1;
}

static void 
free_list(list_t* list) {
  free(list->e);
}

static int 
follow(v2s_t* head, v2s_t* tail) {
  

  // resolve tail
  // is head too far from tail
  int disp_x = head->x - tail->x;
  int disp_y = head->y - tail->y;

  int dist_x = abs_of(disp_x);
  int dist_y = abs_of(disp_y);

  if (dist_x + dist_y >= 3) {
    // diagonals case
    if (disp_x >= 2){
      tail->x += 1;
      if (disp_y < 0)
        tail->y -= 1;
      else 
        tail->y += 1;
    }
    else if (disp_y >= 2){
      tail->y += 1;
      if (disp_x < 0)
        tail->x -= 1;
      else 
        tail->x += 1;
    }
    else if (disp_x <= -2)
    {
      tail->x -= 1;
      if (disp_y < 0) 
        tail->y -=1;
      else
        tail->y +=1;
    }
    else if (disp_y <= -2)
    {
      tail->y -= 1;
      if (disp_x < 0) 
        tail->x -=1;
      else
        tail->x +=1;
    }
  }
  else {
    if (disp_x >= 2) {
      tail->x += 1;
    }
    else if(disp_x <= -2) {
      tail->x -= 1;
    }

    if (disp_y >= 2) {
      tail->y += 1;
    }
    else if(disp_y <= -2) {
      tail->y -= 1;
    }
  }

#if 0 
  printf("Move %c\n", direction);
  printf("head: %d %d\n", head->x, head->y);
  printf("tail: %d %d\n\n", tail->x, tail->y);
#endif

  return 1;
}

#define arr_count(a) (sizeof(a)/sizeof(*(a)))


int main() {
  FILE* fp = fopen("input.txt", "r");
  list_t visited = {0};
 
  if (fp) 
  {
    if (init_list(&visited)) 
    {
      v2s_t nodes[10] = {0};

      insert_unique(&visited, nodes[9]);

      char buffer[10];
      while (fgets(buffer, sizeof(buffer), fp))
      {
        char direction = buffer[0];
        
        int amount = str_to_int(buffer + 2); // super hacky haha 
                                             //
        printf("%c %d\n", direction, amount);
        for(int i = 0; i < amount; ++i) {

          switch(direction) {
            case 'U': {
              nodes[0].y += 1;
            } break;
            case 'D': {
              nodes[0].y -= 1;
            } break;
            case 'L': {
              nodes[0].x -= 1;
            } break;
            case 'R': {
              nodes[0].x += 1;
            } break;
          }

          for (int node_index = 0; node_index < arr_count(nodes)-1; ++node_index) {
            v2s_t* head = nodes + node_index;
            v2s_t* tail = nodes + node_index + 1;

            follow(head,  tail);
          }

#if 0
          for (int node_index = 0; node_index < arr_count(nodes); ++node_index) {
            printf("#%d: %d %d\n", node_index, nodes[node_index].x, nodes[node_index].y);
          }
          printf("\n");
#endif
          if (!insert_unique(&visited, nodes[9])) {
            printf("Died 2\n");
            goto die;
          }
        }
      }

#if 0
      for(int i = 0; i < visited.count; ++i) {
        printf("%d %d\n", visited.e[i].x, visited.e[i].y);
      }
#endif

      printf("Count: %d\n", visited.count);
      
    }
    else {
      printf("Fail to init list\n");
    }
die:
    fclose(fp);
  }

  free_list(&visited);

}
