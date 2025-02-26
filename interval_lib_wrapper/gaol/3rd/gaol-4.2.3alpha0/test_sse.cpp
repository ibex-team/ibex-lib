#include <iostream>

int
main (int argc, char *argv[])
{
  unsigned int a, b, c, d;
  bool have_sse2 = false;
  bool have_sse3 = false;
  __asm__ __volatile__ ("cpuid" : "=a" (a), "=b" (b), "=c" (c), "=d" (d)
                                : "a" (0));
  if (a >= 1) {
    __asm__ __volatile__ ("cpuid" : "=a" (a), "=b" (b), "=c" (c), "=d" (d)
                                  : "a" (1));
    have_sse2 = bool(d & 0x04000000);
    have_sse3 = bool(c & 0x00000001);
  }
  std::cout << "HAVE_SSE2=" << std::boolalpha << have_sse2 << std::endl;
  std::cout << "HAVE_SSE3=" << std::boolalpha << have_sse3 << std::endl;
  return 0;
}
