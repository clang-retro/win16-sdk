# QuickWin console window

A C or C++ program with `main()` can run on 16-bit Windows in a console
window. The window shows the standard output and the standard error of the
program, and it reads the standard input from the keyboard. This is the
same as the QuickWin library of Microsoft C/C++ 7.0 and Visual C++ 1.x.

## Link a QuickWin program

Add `-mquickwin` to the link command:

```sh
clang --target=i8086-pc-win16 -mcmodel=small --sysroot=<sysroot> -Os \
  -mquickwin hello.c -o hello.exe
```

The flag links `<sysroot>/lib/win16/<model>/quickwin.obj` after
`crt0.obj`. The flag has no effect on a DLL. When you compile only (`-c`),
the compiler ignores the flag.

Do not use `-mquickwin` with a program that has `WinMain`. Such a program
makes its own windows.

## What the window does

- The window opens at the first read or write on file descriptor 0, 1 or
  2. A program that does not use them does not open a window.
- The window has 80 columns. It keeps the last 100 lines. Use the scroll
  bar to see the older lines.
- `printf`, `puts`, `fputs` and `write` put text in the window. A tab
  moves the cursor to the next multiple of 8 columns.
- `fgets`, `scanf` and `read` read one line from the keyboard. The user
  types the line in the window. Backspace removes the last character.
  Enter ends the line. Ctrl+Z at the start of a line gives the end of the
  file.
- While the program reads, it lets other Windows programs run.

## When the program ends

When `main` returns or the program calls `exit`, the window does one of
these:

| Mode | Value | Result |
| --- | --- | --- |
| `_WINEXITPROMPT` | 1 | The title changes to "Finished". The window stays until the user closes it. This is the default. |
| `_WINEXITNOPERSIST` | 2 | The window closes. |
| `_WINEXITPERSIST` | 3 | The same as `_WINEXITPROMPT`. |

## Functions

Include `<quickwin.h>`.

| Function | Result |
| --- | --- |
| `int _wsetexit(int mode)` | Sets what the window does when the program ends. Returns 0, or -1 for a mode that is not in the table above. |
| `int _wgetexit(void)` | Returns the mode that `_wsetexit` set. |
| `int _wyield(void)` | Lets Windows send the messages for the window and for other programs. Use it in a long calculation. Returns 1. |

## How it works

The C library (picolibc for Windows) sends file descriptors 0, 1 and 2 to
the functions in the pointers `__win16_console_read` and
`__win16_console_write`. A constructor in `quickwin.obj` sets these
pointers. Without `quickwin.obj`, the pointers are null and the
descriptors go to DOS, as before.

## Limits

- The window has one font, the OEM fixed font (`OEM_FIXED_FONT`).
- The keyboard does not scroll the window. Use the scroll bar.
- The window has no menus, no clipboard and no child windows. The
  QuickWin functions of Microsoft for menus and more windows
  (`_wopen`, `_wmenuclick` and others) are not available.
- The program cannot change the size of the text buffer.
