#include <stdio.h>
#include <stdlib.h>

static void convert_to_utf8(unsigned c)
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
  fputs(x[c], stdout);
}

static unsigned getword(void)
{
  int c1, c2;

  c1 = getchar();
  if (c1 == EOF)
    exit(0);
  c2 = getchar();
  if (c2 == EOF) {
    fprintf(stderr, "End of file inside word.\n");
    exit(1);
  }
  if (c2 & ~0xF) {
    fprintf(stderr, "Bad data in word.\n");
    return 1;
  }
  return (c1 & 0xFF) | (c2 << 4);
}

int main(void)
{
  unsigned word;

  for(;;) {
    word = getword();
    convert_to_utf8(word >> 6);
    convert_to_utf8(word & 077);
  }
}
