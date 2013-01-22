#include <iostream>

extern bool bug016();
extern bool bug017();
extern bool bug018();
extern bool bug019();
extern bool bug020();
extern bool bug021();
extern bool bug022();

using namespace std;


int failed=0;

void _test(int bug_nb, bool mustbetrue) {
  cout << "bug " << bug_nb << '\t';
  if (mustbetrue)
    cout << "ok\n";
  else {
    cout << "FAILED\n";
    failed++;
  }
}

int main() {
  _test(16,bug016());
  _test(17,bug017());
  _test(18,bug018());
  _test(19,bug019());
  _test(20,bug020());
  _test(21,bug021());
  _test(22,bug022());

  cout << endl << endl;
  cout << failed << " failed.\n";

  return 0;
}

