// For printf
#include <stdio.h>

// For malloc/free
#include <stdlib.h>

//
// Types
//
#include <stdint.h>
typedef int32_t s32_t;
typedef uint32_t u32_t;
typedef uint32_t b32_t;

#define array_count(a) sizeof(a)/sizeof(*a)
#define true 1
#define false 0



static b32_t 
is_alpha(char c) {
  return c >= 'a' && c <= 'z' ||
         c >= 'A' && c <= 'Z';
}

static b32_t
is_digit(char c) {
  return c >= '0' && c <= '9';
}

static b32_t 
is_readable(char c) {
  return c != '\r' && c != '\n' && c != 0;
}

static b32_t 
is_uppercase(char c) {
  return c >= 'A' && c <= 'Z';
}


static u32_t 
cstr_count_readables(const char* buffer) {
  u32_t count = 0;
  while(is_readable(*buffer++))
    count++;

  return count;
}

static u32_t 
cstr_to_u32(const char* str) {
  u32_t ret = 0;
  while(*str >= '0' && *str <= '9') {
    ret *= 10;
    ret += (*str - '0'); 
    ++str; 
  }
  return ret;
}

static u32_t 
cstr_len_ext(const char* str, b32_t (*pred)(char))
{
  // common strlen that counts what I care
  u32_t ret = 0;
  while(pred(*str)) 
  {
    ++str;
    ++ret;
  }

  return ret;
}

//
// Range
//
typedef struct {
  u32_t min, max;
} r2u_t;

static b32_t
r2u_is_overlapping(r2u_t lhs, r2u_t rhs) {
  // There exists 'x' such that:
  //   lhs.min <= x <= lhs.max
  //   rhs.min <= x <= rhs.max
  // Thus:
  return lhs.min <= rhs.max && rhs.min <= lhs.max;

}

static int 
r2u_is_fully_overlapping(r2u_t lhs, r2u_t rhs) {
  int is_lhs_overlapping = lhs.min <= rhs.min && lhs.max >= rhs.max;
  int is_rhs_overlapping = rhs.min <= lhs.min && rhs.max >= lhs.max; 

  return is_lhs_overlapping || is_rhs_overlapping;
}

//
// Tokenizer
//
typedef struct tokenizer_t {
  char* buffer;
  u32_t at;
} tokenizer_t;

// terrible algorithm that will break if input is bad
// in the case of this qn, do NOT call more than 4 times
static u32_t 
tokenizer_get_next_u32(tokenizer_t* t) {
  // go forward until we find a digit
  while(!is_digit(t->buffer[t->at])) ++(t->at);

  u32_t num = 0;
  while(is_digit(t->buffer[t->at])) {
    num *= 10;
    num += t->buffer[t->at] - '0'; 
    t->at++;
  }
  return num;
}

//
// Answers
//
#include "aoc_d1.h"
#include "aoc_d2.h"
#include "aoc_d3.h"
#include "aoc_d4.h"
#include "aoc_d5.h"


