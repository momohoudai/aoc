#include <stdio.h>
#include <stdlib.h>

#define null (0)

typedef enum node_type_t {
  NODE_TYPE_FILE,
  NODE_TYPE_DIR,
} node_type_t;

typedef struct node_t {
  node_type_t type;

  // we are either a file, which we only care about size, which is an int
  // or we are a directory, which points to a node;
  union {
    int size; 
    struct node_t* head;
  };
  char name[128];
  struct node_t* next;

  struct node_t* parent;
} node_t;


#if 0
static node_t make_file(const char* name, int size){
  node_t ret; 
  ret.node_type = NODE_TYPE_FILE;
  ret.name = name;
  ret.size = size;

  return ret;
}

static node_t make_dir(const char* name) {
  node_t ret;
  ret.node_type = NODE_TYPE_DIR;
  ret.name = name;
  ret.head = null;

  return ret;
}
#endif

typedef struct tokenizer_t {
  char* buffer;
  int at;
} tokenizer_t;

typedef struct token_t {
  int start, ope;
} token_t;

static int is_whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\r';
}

static int is_digit(char c) {
  return c >= '0' &&  
         c <= '9';
}

static token_t next_token(tokenizer_t* t) {
  token_t ret = {0};

  // consume all white spaces
  while(is_whitespace(t->buffer[t->at])) 
    ++(t->at);

  if (t->buffer[t->at] == 0) {
    return ret;
  }

  ret.start = t->at; 

  while(!is_whitespace(t->buffer[t->at])) 
    ++(t->at)

  ret.ope = t->at;
  
  return ret;
}

static int token_cmp(tokenizer_t tokenizer, token_t token, const char* str) {
  while(*str != 0) {
    if (*str != tokenizer.buffer[token.start]) { 
      return 0;
    }
    str++;
    token.start++;
  }
  return 1;
}
// unsafe but whatever
static int cstr_copy(char* dest, const char* src, int start, int ope) {
  int j = 0;
  for(int i = start; i < ope; ++i, ++j) {
    dest[j] = src[i];
  }
  dest[j] = 0;
}

static int token_to_int(tokenizer_t t, token_t token) {
  // very unsafe function, don't anyhow use! 
  int num = 0;
  for(int it = token.start; it < token.ope; ++it)
  {
    num *= 10;
    num += t.buffer[it] - '0'; 
  }
  return num;
}
#if 0
static int get_next_number(tokenizer_t* t) {
  // go forward until we find a digit
  while(!is_digit(t->buffer[t->at])) ++(t->at);

  int num = 0;
  while(is_digit(t->buffer[t->at])) {
    num *= 10;
    num += t->buffer[t->at] - '0'; 
    t->at++;
  }
  return num;
}
#endif

static void
traverse(node_t* node) {
  static int scope = 0;
  node_t* itr = node; 
  while(itr) {
    if (itr->type == NODE_TYPE_FILE) {
      for(int i = 0; i < scope; ++i) printf(" ");
      printf("file (%10s) (%d)\n", itr->name, itr->size);
    }
    else{
      for(int i = 0; i < scope; ++i) printf(" ");
      printf("dir  (%10s)\n", itr->name);
      
      scope+=2;
      traverse(itr->head);
      scope-=2;

    }
    itr = itr->next;
  }

}
static int
traverse_for_solution(node_t* node, int* dir_sizes) {
  static int scope = 0;
  node_t* itr = node; 
  int ret = 0;
  while(itr) {

    if (itr->type == NODE_TYPE_FILE) {
      for(int i = 0; i < scope; ++i) printf(" ");
      printf("file (%10s) (%d)\n", itr->name, itr->size);
      
      ret += itr->size;
    }
    else{
      for(int i = 0; i < scope; ++i) printf(" ");
      
      scope+=2;
      printf("dir  (%10s)\n", itr->name);
      int sum = traverse_for_solution(itr->head, dir_sizes);
      ret += sum;

      if (sum <= 100000)
        *dir_sizes += sum;

      scope-=2;
      for(int i = 0; i < scope; ++i) printf(" ");
      printf("(%d) -> %d\n", sum, *dir_sizes);

    }
    itr = itr->next;
  }
  return ret;

}

int main() {
#define make_node() (nodes + node_count++);
#define push_node(head, node) (node)->next = (head); (head) = (node);
  node_t nodes[512] = {0};
  int node_count = 0;

  node_t* cur_dir = make_node();
  cur_dir->type = NODE_TYPE_DIR;
  cur_dir->name[0] = '/';

  FILE* fp = fopen("input.txt", "r");
  char buffer[128] = {0};
  if (fp) {

    while(fgets(buffer, sizeof(buffer), fp)) {

      ////////////////////////////////////////////
      // is a command
      if (buffer[0] == '$') 
      {
        printf("command: ");
        tokenizer_t tokenizer = { buffer, 0 };

        // get rid of $
        next_token(&tokenizer);

        token_t command = next_token(&tokenizer);
        if (token_cmp(tokenizer, command, "cd")) {
          token_t dir_name =  next_token(&tokenizer);
          if (token_cmp( tokenizer, dir_name, "/")) {

            cur_dir = nodes + 0;
          }

          else if (token_cmp( tokenizer, dir_name, "..")) {
            if (cur_dir->parent) 
              cur_dir = cur_dir->parent;
          }
          else {
            // traverse the directory and set cur_dir to the node
            for(node_t* itr = cur_dir->head; itr; itr = itr->next) 
            {
              if (itr->type == NODE_TYPE_DIR) {
                if (token_cmp(tokenizer, dir_name, itr->name))
                {
                  // we found our dude
                  cur_dir = itr;
                  break;
                }


              }
            }

          }


        }
        else if (token_cmp(tokenizer, command, "ls"))
        {
          //printf( "ls");
        }
        else {
          //printf("DIE");
        }
          

        printf("\n");

      }

      /////////////////////////////////////////////
      // Not a command. There is only two cases. Either:
      // - We are listing a file
      // - We are listing a directory
      else {
        tokenizer_t tokenizer = { buffer, 0 };
        token_t token = next_token(&tokenizer);
        if(is_digit(tokenizer.buffer[0])){
          // we are listing a file property here
          // currently the token should be containing a value
          node_t* node = make_node();
          node->parent = cur_dir;
          node->size = token_to_int(tokenizer, token);
          node->type = NODE_TYPE_FILE;
          
          // we expect the 2nd token to be a name
          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);


          push_node(cur_dir->head, node);
          printf("File found: (%s), (%d)\n", node->name, node->size);

        }

        else {
          // we are listing directory property here
          node_t* node = make_node();
          node->type = NODE_TYPE_DIR;
          node->parent = cur_dir;

          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);

          push_node(cur_dir->head, node);
          printf("Dir found: (%s)\n", node->name);
        }

      }

    }
    fclose(fp);
  }


  printf( "=== Checking contents ===\n");
  traverse(nodes + 0);
    

  printf( "=== Checking contents ===\n");
  int dir_sizes = 0;
  traverse_for_solution(nodes, &dir_sizes);
  printf("solution: %d\n", dir_sizes);



}

