// OK
void fun(int x, int y, int z);
int a[] = {5, 6, 7};
class A : public B,
          public C
{
     // ...
};
void operator,(const A &left, const A &right);
void operator , (const A &left, const A &right);
// not OK
void fun(int x , int y , int z);
int a[] = {5 , 6 , 7};
class A : public B ,
          public C
{
     // ...
};
