static void directory_index(void)
{
}

static void save_manuscript(void)
{
}

static void add_manuscript(void)
{
}

static void save_binary(void)
{
}

static void load_binary(void)
{
}

static const struct {
  const char *name;
  void (*fn)(void);
} command[] = {
  { "dx", directory_index },
  { "sm", save_manuscript },
  { "am", add_manuscript },
  { "sb", save_binary },
  { "lo", load_binary }
};
