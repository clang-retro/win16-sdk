/* quickwin.h: the console window of a C program on 16-bit Windows (clang
 * -mquickwin), in the manner of Microsoft's QuickWin library. Written for
 * clang-dos; AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * With -mquickwin, the standard input, output and error of a program with
 * main() go to a window. These functions control that window. */
#ifndef _INC_QUICKWIN
#define _INC_QUICKWIN

#ifdef __cplusplus
extern "C" {
#endif

/* What the window does when the program ends (_wsetexit). */
#define _WINEXITPROMPT 1    /* "Finished" in the title; the user closes it */
#define _WINEXITNOPERSIST 2 /* the window closes */
#define _WINEXITPERSIST 3   /* as _WINEXITPROMPT */

int _wsetexit(int mode);
int _wgetexit(void);
int _wyield(void);

#ifdef __cplusplus
}
#endif

#endif /* _INC_QUICKWIN */
