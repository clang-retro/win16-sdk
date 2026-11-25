/* quickwin.c: a console window for C programs on 16-bit Windows, in the
 * manner of Microsoft's QuickWin library. Written for clang-dos;
 * AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * A program with main() that is linked with -mquickwin (which links this
 * file as quickwin.obj) gets a window for its standard input, output and
 * error. printf, puts and the rest write into the window, and scanf, gets
 * and fgets read lines typed into it. The window keeps the last
 * QW_LINES lines, with a scroll bar. When main returns, the title says
 * "Finished" and the window stays until the user closes it (see
 * _wsetexit in quickwin.h).
 *
 * The C library sends file descriptors 0, 1 and 2 here through the hooks
 * __win16_console_read and __win16_console_write, which the constructor
 * below sets. The window opens at the first input or output. */
#include <windows.h>
#include <quickwin.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define QW_COLS 80
#define QW_LINES 100
#define QW_KEYS 64
#define QW_TAB 8

extern ssize_t (*__win16_console_read)(int fd, void *buf, size_t count);
extern ssize_t (*__win16_console_write)(int fd, const void *buf,
                                        size_t count);
extern unsigned short __win16_hinstance;
extern unsigned short __win16_cmdshow;

static char *text;             /* QW_LINES lines of QW_COLS characters */
static int first;              /* the line of text that is the oldest */
static int nlines;             /* lines in use, 1 or more */
static int col;                /* the cursor column in the last line */
static int top;                /* the first line in the window */
static int rows = 25;          /* lines that the window shows */
static int cw = 8, ch = 16;    /* the size of a character */
static HWND wnd;
static BOOL opened;            /* the window was made (it may be closed) */
static BOOL focus;
static char keys[QW_KEYS];     /* typed characters, not yet read */
static int khead, ktail;
static char pending[QW_COLS + 2]; /* the rest of an input line */
static int pend_pos, pend_len;
static int exit_mode = _WINEXITPROMPT;
static char title[80];

/* Returns the start of line i, where line 0 is the oldest. */
static char *line(int i) {
  return text + ((first + i) % QW_LINES) * QW_COLS;
}

/* Puts the caret at the cursor, if the window has the focus. */
static void place_caret(void) {
  if (focus)
    SetCaretPos(col * cw, (nlines - 1 - top) * ch);
}

/* Sets the scroll bar to the lines in use and the first line shown. */
static void set_scroll(void) {
  int max = nlines - rows;
  if (max < 0)
    max = 0;
  SetScrollRange(wnd, SB_VERT, 0, max, FALSE);
  SetScrollPos(wnd, SB_VERT, top, TRUE);
}

/* Shows the last line, and repaints the window. */
static void show_end(void) {
  int t = nlines - rows;
  top = t < 0 ? 0 : t;
  set_scroll();
  InvalidateRect(wnd, NULL, TRUE);
  place_caret();
}

/* Starts a new line at the end. When the buffer is full, the oldest line
 * goes. */
static void new_line(void) {
  if (nlines == QW_LINES)
    first = (first + 1) % QW_LINES;
  else
    nlines++;
  memset(line(nlines - 1), ' ', QW_COLS);
  col = 0;
}

/* Writes the character c at the cursor, with the control characters of a
 * terminal: newline, carriage return, backspace, tab and bell. */
static void put_char(char c) {
  switch (c) {
  case '\n':
    new_line();
    return;
  case '\r':
    col = 0;
    return;
  case '\b':
    if (col > 0)
      col--;
    return;
  case '\t':
    do
      put_char(' ');
    while (col % QW_TAB);
    return;
  case '\a':
    MessageBeep(0);
    return;
  }
  if (col == QW_COLS)
    new_line();
  line(nlines - 1)[col++] = c;
}

/* Handles the messages of the window. */
static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_SIZE:
    rows = HIWORD(lp) / ch;
    if (rows < 1)
      rows = 1;
    show_end();
    return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);
    SelectObject(dc, GetStockObject(OEM_FIXED_FONT));
    SetBkColor(dc, GetSysColor(COLOR_WINDOW));
    SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
    for (int r = 0; r < rows && top + r < nlines; r++)
      TextOut(dc, 0, r * ch, line(top + r), QW_COLS);
    EndPaint(hwnd, &ps);
    return 0;
  }
  case WM_CHAR:
    if ((ktail + 1) % QW_KEYS != khead) {
      keys[ktail] = (char)wp;
      ktail = (ktail + 1) % QW_KEYS;
    } else {
      MessageBeep(0);
    }
    return 0;
  case WM_VSCROLL: {
    int t = top, max = nlines - rows;
    switch (wp) {
    case SB_LINEUP:
      t--;
      break;
    case SB_LINEDOWN:
      t++;
      break;
    case SB_PAGEUP:
      t -= rows;
      break;
    case SB_PAGEDOWN:
      t += rows;
      break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
      t = LOWORD(lp);
      break;
    case SB_TOP:
      t = 0;
      break;
    case SB_BOTTOM:
      t = max;
      break;
    }
    if (t > max)
      t = max;
    if (t < 0)
      t = 0;
    if (t != top) {
      top = t;
      SetScrollPos(hwnd, SB_VERT, top, TRUE);
      InvalidateRect(hwnd, NULL, TRUE);
      place_caret();
    }
    return 0;
  }
  case WM_SETFOCUS:
    focus = TRUE;
    CreateCaret(hwnd, 0, cw, 2);
    place_caret();
    ShowCaret(hwnd);
    return 0;
  case WM_KILLFOCUS:
    focus = FALSE;
    DestroyCaret();
    return 0;
  case WM_DESTROY:
    wnd = 0;
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, msg, wp, lp);
}

/* Handles the messages that wait. With wait set, it waits for one message
 * first. When the user closes the window while the program runs, the
 * program ends. */
static void pump(BOOL wait) {
  MSG m;
  if (wait) {
    if (!GetMessage(&m, 0, 0, 0))
      goto quit;
    TranslateMessage(&m);
    DispatchMessage(&m);
  }
  while (PeekMessage(&m, 0, 0, 0, PM_REMOVE)) {
    if (m.message == WM_QUIT)
      goto quit;
    TranslateMessage(&m);
    DispatchMessage(&m);
  }
  return;
quit:
  exit_mode = _WINEXITNOPERSIST;
  exit(0);
}

/* Runs at exit: says "Finished" in the title, and waits for the user to
 * close the window, as _wsetexit chose. */
static void at_exit(void) {
  MSG m;
  if (!wnd || exit_mode == _WINEXITNOPERSIST) {
    if (wnd)
      DestroyWindow(wnd);
    return;
  }
  lstrcpy(title + lstrlen(title), " [Finished]");
  SetWindowText(wnd, title);
  while (wnd && GetMessage(&m, 0, 0, 0)) {
    TranslateMessage(&m);
    DispatchMessage(&m);
  }
}

/* Makes the window, the first time. Returns FALSE if it cannot, or if the
 * user has closed it. */
static BOOL open_window(void) {
  static const char cls[] = "QuickWinConsole";
  WNDCLASS wc;
  RECT r;
  HDC dc;
  TEXTMETRIC tm;
  char *name, *p;

  if (opened)
    return wnd != 0;
  opened = TRUE;
  text = malloc(QW_LINES * QW_COLS);
  if (!text)
    return FALSE;
  nlines = 1;
  memset(text, ' ', QW_COLS);

  memset(&wc, 0, sizeof(wc));
  wc.lpfnWndProc = wnd_proc;
  wc.hInstance = (HINSTANCE)__win16_hinstance;
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszClassName = cls;
  RegisterClass(&wc); /* fails, harmlessly, for a second instance */

  dc = GetDC(0);
  SelectObject(dc, GetStockObject(OEM_FIXED_FONT));
  GetTextMetrics(dc, &tm);
  ReleaseDC(0, dc);
  cw = tm.tmAveCharWidth;
  ch = tm.tmHeight + tm.tmExternalLeading;

  /* The title is the program's file name. */
  GetModuleFileName((HINSTANCE)__win16_hinstance, title, sizeof(title) - 12);
  for (name = p = title; *p; p++)
    if (*p == '\\' || *p == ':')
      name = p + 1;
  memmove(title, name, lstrlen(name) + 1);

  r.left = r.top = 0;
  r.right = QW_COLS * cw + GetSystemMetrics(SM_CXVSCROLL);
  r.bottom = rows * ch;
  AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
  wnd = CreateWindow(cls, title, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
                     CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left,
                     r.bottom - r.top, 0, 0, (HINSTANCE)__win16_hinstance,
                     NULL);
  if (!wnd)
    return FALSE;
  ShowWindow(wnd, __win16_cmdshow ? __win16_cmdshow : SW_SHOWNORMAL);
  UpdateWindow(wnd);
  atexit(at_exit);
  return TRUE;
}

/* Writes count bytes of buf into the window, for file descriptors 1 and 2
 * (and 0). Returns count, or -1 if there is no window. */
static ssize_t console_write(int fd, const void *buf, size_t count) {
  const char *p = buf;
  (void)fd;
  if (!open_window())
    return -1;
  HideCaret(wnd);
  for (size_t i = 0; i < count; i++)
    put_char(p[i]);
  show_end();
  ShowCaret(wnd);
  UpdateWindow(wnd);
  pump(FALSE);
  return count;
}

/* Echoes c and repaints the cursor line. */
static void echo(char c) {
  put_char(c);
  show_end();
  UpdateWindow(wnd);
}

/* Reads a line that the user types into the window, with backspace, for
 * file descriptor 0. Returns at most count bytes; the rest of the line is
 * kept for the next read. Ctrl-Z at the start of a line is the end of the
 * file. Returns 0 at the end of the file, or -1 if there is no window. */
static ssize_t console_read(int fd, void *buf, size_t count) {
  (void)fd;
  if (!open_window())
    return -1;
  if (pend_pos == pend_len) {
    pend_pos = pend_len = 0;
    for (;;) {
      char c;
      while (khead == ktail) {
        pump(TRUE);
        if (!wnd)
          return 0;
      }
      c = keys[khead];
      khead = (khead + 1) % QW_KEYS;
      if (c == '\r') {
        pending[pend_len++] = '\n';
        echo('\n');
        break;
      }
      if (c == 0x1a && pend_len == 0)
        return 0;
      if (c == '\b') {
        if (pend_len > 0 && col > 0) {
          pend_len--;
          echo('\b');
          echo(' ');
          echo('\b');
        }
        continue;
      }
      if ((unsigned char)c < ' ' || pend_len == QW_COLS) {
        MessageBeep(0);
        continue;
      }
      pending[pend_len++] = c;
      echo(c);
    }
  }
  if (count > (size_t)(pend_len - pend_pos))
    count = pend_len - pend_pos;
  memcpy(buf, pending + pend_pos, count);
  pend_pos += count;
  return count;
}

/* Sends the standard input, output and error of the program to the
 * window, before main runs. */
__attribute__((constructor)) static void install(void) {
  __win16_console_read = console_read;
  __win16_console_write = console_write;
}

/* Chooses what the window does when the program ends. */
int _wsetexit(int mode) {
  if (mode != _WINEXITPROMPT && mode != _WINEXITNOPERSIST &&
      mode != _WINEXITPERSIST)
    return -1;
  exit_mode = mode;
  return 0;
}

/* Returns the current choice of _wsetexit. */
int _wgetexit(void) { return exit_mode; }

/* Lets Windows run other programs and repaint the window, from a long
 * computation that does no input or output. */
int _wyield(void) {
  if (wnd)
    pump(FALSE);
  return 1;
}
