//
// Copyright (C) 2004-2006, Maciej Sobczak
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//

// Note: this file is not supposed to be a stand-alone header


template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
struct construct
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7,
          T8 t8, T9 t9)
     { return new C(t1, t2, t3, t4, t5, t6, t7, t8, t9); }
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
struct construct<C, T1, T2, T3, T4, T5, T6, T7, T8, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
     { return new C(t1, t2, t3, t4, t5, t6, t7, t8); }
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
struct construct<C, T1, T2, T3, T4, T5, T6, T7, void, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
     { return new C(t1, t2, t3, t4, t5, t6, t7); }
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
struct construct<C, T1, T2, T3, T4, T5, T6, void, void, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
     { return new C(t1, t2, t3, t4, t5, t6); }
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5>
struct construct<C, T1, T2, T3, T4, T5, void, void, void, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
     { return new C(t1, t2, t3, t4, t5); }
};

template <class C, typename T1, typename T2, typename T3, typename T4>
struct construct<C, T1, T2, T3, T4, void, void, void, void, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3, T4 t4)
     { return new C(t1, t2, t3, t4); }
};

template <class C, typename T1, typename T2, typename T3>
struct construct<C, T1, T2, T3, void, void, void, void, void, void>
{
     static C * doit(T1 t1, T2 t2, T3 t3)
     { return new C(t1, t2, t3); }
};

template <class C, typename T1, typename T2>
struct construct<C, T1, T2, void, void, void, void, void, void, void>
{
     static C * doit(T1 t1, T2 t2)
     { return new C(t1, t2); }
};

template <class C, typename T1>
struct construct<C, T1, void, void, void, void, void, void, void, void>
{
     static C * doit(T1 t1)
     { return new C(t1); }
};

template <class C>
struct construct<C, void, void, void, void, void,
                 void, void, void, void>
{
     static C * doit()
     { return new C(); }
};
