/* Calls EHDLL.DLL twice, so that its second call starts with the state
 * that the first one left. */
#include <stdio.h>

int __far __pascal EhWork(void);

/* Prints ALL PASSED when both calls report no failure. */
int main(void) {
  int r = EhWork();
  if (!r)
    r = EhWork();
  if (r)
    printf("EhWork: %#x\n", r);
  printf(r ? "FAILED\n" : "ALL PASSED\n");
  return r != 0;
}
