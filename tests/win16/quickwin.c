/* A QuickWin program (-mquickwin): output into the window, and a line of
 * input that the test types into it with WM_CHAR messages. The window is
 * not stdout, so the result goes to quickwin.out. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <quickwin.h>

static int failures;
#define CHECK(c)                                                               \
  do {                                                                         \
    if (!(c)) {                                                                \
      fprintf(log, "FAIL line %d: %s\n", __LINE__, #c);                        \
      failures++;                                                              \
    }                                                                          \
  } while (0)

/* Writes to the window, types "Ada" and Enter into it, reads the line
 * back, and writes ALL PASSED to quickwin.out when every check holds. */
int main(void) {
  FILE *log = fopen("quickwin.out", "w");
  char name[32];
  HWND w;

  if (!log)
    return 1;
  printf("Hello from QuickWin\n");
  for (int i = 1; i <= 30; i++)
    printf("line %d\twith a tab\n", i);
  CHECK(isatty(1));
  w = FindWindow("QuickWinConsole", NULL);
  CHECK(w != 0);
  for (const char *p = "Ada\r"; *p; p++)
    PostMessage(w, WM_CHAR, *p, 0);
  printf("Your name? ");
  fflush(stdout);
  CHECK(fgets(name, sizeof(name), stdin) != NULL);
  CHECK(strcmp(name, "Ada\n") == 0);
  printf("Hello, %s", name);
  CHECK(_wsetexit(_WINEXITNOPERSIST) == 0);
  CHECK(_wgetexit() == _WINEXITNOPERSIST);
  fprintf(log, failures ? "FAILED\n" : "ALL PASSED\n");
  fclose(log);
  return failures;
}
