// OK
void fun()
{
     if (...)
     {
          return;
     }
     // ...
     throw e;
     return (x == y);
}
int add(int a, int b) throw()
{
     return a + b;
}
// not OK
void fun()
{
     if (...)
     {
          return ;
     }
     // ...
     throw(e);
     return(x == y);
}
int add(int a, int b) throw()
{
     return(a + b);
}
