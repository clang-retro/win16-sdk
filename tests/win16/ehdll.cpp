/* C++ exceptions inside a DLL, in every memory model. The DLL throws and
 * catches its own exceptions; they do not cross to the program. In the
 * small and medium models, the objects whose address escapes are in
 * DGROUP, and the unwinder finds the frames of each task by its SS. */
#include <stdexcept>
#include <string>
#include <vector>

#define API __export __far __pascal

namespace {
int dtors;

/* Counts its destructions; its address escapes to a function that is not
 * inlined. */
struct Guard {
  std::string name;
  explicit Guard(const char *n) : name(n) {}
  ~Guard() { dtors++; }
};

/* Uses g so that its address escapes. */
__attribute__((noinline)) void touch(Guard &g) { g.name += '.'; }

/* Throws a std::runtime_error for n > 2, an int for n == 2, nothing
 * otherwise, with a Guard on the way. */
__attribute__((noinline)) void thrower(int n) {
  Guard g("thrower");
  touch(g);
  if (n > 2)
    throw std::runtime_error("big");
  if (n == 2)
    throw n;
}

/* Catches, then throws again to the caller. */
__attribute__((noinline)) void rethrower() {
  Guard g("rethrower");
  touch(g);
  try {
    thrower(3);
  } catch (const std::exception &) {
    throw;
  }
}
} // namespace

/* Returns 0 when all of it works, or a bit for each failure. */
extern "C" int API EhWork(void) {
  int bad = 0;
  dtors = 0;
  try {
    thrower(5);
    bad |= 1;
  } catch (const std::exception &e) {
    if (std::string(e.what()) != "big")
      bad |= 2;
  }
  try {
    thrower(2);
    bad |= 4;
  } catch (int v) {
    if (v != 2)
      bad |= 8;
  }
  try {
    thrower(0);
  } catch (...) {
    bad |= 16;
  }
  try {
    rethrower();
    bad |= 32;
  } catch (const std::runtime_error &) {
    try {
      std::vector<int> v(3);
      (void)v.at(7);
      bad |= 64;
    } catch (const std::out_of_range &) {
    }
  }
  if (dtors != 5)
    bad |= 128;
  return bad;
}
