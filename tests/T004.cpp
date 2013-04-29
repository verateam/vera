// OK
class A : public B, private C
{
public:
     A();
     ~A();
protected:
     // ...
private:
     // ...
};

void fun(int a)
{
     switch (a)
     {
     // ...
     default:
          exit(0);
     }
}
// not OK
class A : public B, private C
{
public :
     A();
     ~A();
protected :
     // ...
private :
     // ...
};

void fun(int a)
{
     switch (a)
     {
     // ...
     default :
          exit(0);
     }
}
