// OK
class MyException {};

struct MyPair
{
    int a;
    int b;
};

enum state { close, open };

enum colors
{
    black,
    red,
    green,
    blue,
    white
};
// not OK
class MyException {
};

struct MyPair {
    int a;
    int b;
};

enum state { close,
  open };

enum colors
{
    black,
    red,
    green,
    blue,
    white
  };
