#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define BLOCK_WORDS  256
#define EMPTY  05757

typedef unsigned short word_t;

static word_t directory[2 * BLOCK_WORDS];

static void fail(const char *message)
{
  fprintf(stderr, "ERROR: %s\n", message);
  exit(1);
}

static const char *convert_to_utf8(word_t c)
{
  static const char *x[] = {
    "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",
    "8",  "9",  "\n", "\b", " ",  "i",  "p",  "-",
    "+",  "|",  "#",  NULL, "A",  "B",  "C",  "D",
    "E",  "F",  "G",  "H",  "I",  "J",  "K",  "L",
    "M",  "N",  "O",  "P",  "Q",  "R",  "S",  "T",
    "U",  "V",  "W",  "X",  "Y",  "Z",  NULL, "→",
    "?",  "=",  "u",  ",",  ".",  "⊟",  "[",  "_",
    "\"", "„",  "<",  ">",  "]",  "ˣ",  ":",  "ʸ"
  };
  return x[c];
}

static int tape_seek(int block)
{
  long offset = BLOCK_WORDS * block;
  offset *= 2;
  return fseek(stdin, offset, SEEK_SET);
}

static word_t get_word(void)
{
  unsigned char data[2];
  word_t word;
  if (fread(data, 1, 2, stdin) != 2)
    fail("Reading block.");
  if (data[1] & 0xF0)
    fail("Bad data in block.");
  word = data[1];
  word <<= 8;
  word |= data[0];
  return word;
}

static void get_block(int block, word_t *data)
{
  int i;
  if (tape_seek(block) == -1)
    fail("Seeking to block.");
  for (i = 0; i < BLOCK_WORDS; i++)
    data[i] = get_word();
}

static int unused(word_t *data)
{
  return (data[0] == EMPTY && data[1] == EMPTY &&
          data[2] == EMPTY && data[3] == EMPTY &&
          data[4] == EMPTY && data[5] == EMPTY &&
          data[6] == EMPTY && data[7] == EMPTY) ||
         (data[0] == 0     && data[1] == 0 &&
          data[2] == 0     && data[3] == 0 &&
          data[4] == 0     && data[5] == 0 &&
          data[6] == 0     && data[7] == 0);
}

static void entry_name(word_t *data)
{
  int i;
  for (i = 0; i < 4; i++) {
    word_t word = data[i];
    word_t c = word >> 6;
    if (c == 077)
      fputc(' ', stdout);
    else
      fputs(convert_to_utf8(c), stdout);
    c = word & 077;
    if (c == 077)
      fputc(' ', stdout);
    else
      fputs(convert_to_utf8(c), stdout);
  }
}

static void entry(word_t *data)
{
  if (unused(data))
    return;
  entry_name(data);
  if (data[4] != EMPTY && data[5] != EMPTY)
    fprintf(stdout, "  M %03o %-3o", data[4], data[5]);
  if (data[6] != EMPTY && data[7] != EMPTY)
    fprintf(stdout, "  B %03o %-3o", data[6], data[7]);
  printf("\n");
}

static void traverse_index(void)
{
  int i;
  for (i = 8; i < 512; i += 8)
    entry(&directory[i]);
}

int main(int argc, char **argv)
{
  int directory_block = 0426;
  get_block(directory_block, directory);
  if (!unused(&directory[0]))
    fail("Directory block has wrong format.");
  traverse_index();
}
