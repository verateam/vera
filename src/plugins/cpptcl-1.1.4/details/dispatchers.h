//
// Copyright (C) 2004-2006, Maciej Sobczak
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//

// Note: this file is not supposed to be a stand-alone header


// the dispatch class is used to execute the callback functor and to
// capture its return value
// further dispatch<void> specialization ignores the res

template <typename R>
struct dispatch
{
     template <class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f)
     {
          R res = f();
          set_result(interp, res);
     }

     template <typename T1, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f, T1 t1)
     {
          R res = f(t1);
          set_result(interp, res);
     }

     template <typename T1, typename T2, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f, T1 t1, T2 t2)
     {
          R res = f(t1, t2);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3)
     {
          R res = f(t1, t2, t3);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4)
     {
          R res = f(t1, t2, t3, t4);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
     {
          R res = f(t1, t2, t3, t4, t5);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
     {
          R res = f(t1, t2, t3, t4, t5, t6);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
     {
          R res = f(t1, t2, t3, t4, t5, t6, t7);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
     {
          R res = f(t1, t2, t3, t4, t5, t6, t7, t8);
          set_result(interp, res);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          class Functor>
     static void do_dispatch(Tcl_Interp *interp, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
     {
          R res = f(t1, t2, t3, t4, t5, t6, t7, t8, t9);
          set_result(interp, res);
     }
};

template <>
struct dispatch<void>
{
     template <class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f)
     {
          f();
     }

     template <typename T1, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f, T1 t1)
     {
          f(t1);
     }

     template <typename T1, typename T2, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f, T1 t1, T2 t2)
     {
          f(t1, t2);
     }

     template <typename T1, typename T2, typename T3, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3)
     {
          f(t1, t2, t3);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4)
     {
          f(t1, t2, t3, t4);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
     {
          f(t1, t2, t3, t4, t5);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
     {
          f(t1, t2, t3, t4, t5, t6);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
     {
          f(t1, t2, t3, t4, t5, t6, t7);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
     {
          f(t1, t2, t3, t4, t5, t6, t7, t8);
     }

     template <typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          class Functor>
     static void do_dispatch(Tcl_Interp *, Functor f,
          T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
     {
          f(t1, t2, t3, t4, t5, t6, t7, t8, t9);
     }
};
