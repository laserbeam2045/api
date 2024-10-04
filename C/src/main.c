#include "Mode.h"

int main()
{
  Mode mode;

  Mode_init(&mode);
  Mode_run(&mode);
  Mode_finish(&mode);

  return 0;
}
