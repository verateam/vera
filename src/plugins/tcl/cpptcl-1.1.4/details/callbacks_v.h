//
// Copyright (C) 2004-2006, Maciej Sobczak
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//

// Note: this file is not supposed to be a stand-alone header


template <typename R>
class callback1<R, object const &> : public callback_base
{
     typedef object const & T1;
     typedef R (*functor_type)(T1);
     enum { var_start = 1 };
     
public:
     callback1(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t1 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1>(interp, f_,
               t1);
     }

private:
     functor_type f_;
};

template <typename R, typename T1>
class callback2<R, T1, object const &> : public callback_base
{
     typedef object const & T2;
     typedef R (*functor_type)(T1, T2);
     enum { var_start = 2 };
     
public:
     callback2(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t2 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2>(interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               t2);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2>
class callback3<R, T1, T2, object const &> : public callback_base
{
     typedef object const & T3;
     typedef R (*functor_type)(T1, T2, T3);
     enum { var_start = 3 };
     
public:
     callback3(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t3 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3>(interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               t3);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3>
class callback4<R, T1, T2, T3, object const &> : public callback_base
{
     typedef object const & T4;
     typedef R (*functor_type)(T1, T2, T3, T4);
     enum { var_start = 4 };
     
public:
     callback4(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t4 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3, T4>(interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               t4);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3, typename T4>
class callback5<R, T1, T2, T3, T4, object const &> : public callback_base
{
     typedef object const & T5;
     typedef R (*functor_type)(T1, T2, T3, T4, T5);
     enum { var_start = 5 };
     
public:
     callback5(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t5 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5>(
               interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               tcl_cast<T4>::from(interp, objv[4]),
               t5);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3, typename T4,
     typename T5>
class callback6<R, T1, T2, T3, T4, T5, object const &> : public callback_base
{
     typedef object const & T6;
     typedef R (*functor_type)(T1, T2, T3, T4, T5, T6);
     enum { var_start = 6 };
     
public:
     callback6(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t6 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6>(
               interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               tcl_cast<T4>::from(interp, objv[4]),
               tcl_cast<T5>::from(interp, objv[5]),
               t6);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3, typename T4,
     typename T5, typename T6>
class callback7<R, T1, T2, T3, T4, T5, T6, object const &>
     : public callback_base
{
     typedef object const & T7;
     typedef R (*functor_type)(T1, T2, T3, T4, T5, T6, T7);
     enum { var_start = 7 };
     
public:
     callback7(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t7 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6, T7>(
               interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               tcl_cast<T4>::from(interp, objv[4]),
               tcl_cast<T5>::from(interp, objv[5]),
               tcl_cast<T6>::from(interp, objv[6]),
               t7);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3, typename T4,
     typename T5, typename T6, typename T7>
class callback8<R, T1, T2, T3, T4, T5, T6, T7, object const &>
     : public callback_base
{
     typedef object const & T8;
     typedef R (*functor_type)(T1, T2, T3, T4, T5, T6, T7, T8);
     enum { var_start = 8 };
     
public:
     callback8(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t8 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6, T7, T8>(
               interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               tcl_cast<T4>::from(interp, objv[4]),
               tcl_cast<T5>::from(interp, objv[5]),
               tcl_cast<T6>::from(interp, objv[6]),
               tcl_cast<T7>::from(interp, objv[7]),
               t8);
     }

private:
     functor_type f_;
};

template <typename R, typename T1, typename T2, typename T3, typename T4,
     typename T5, typename T6, typename T7, typename T8>
class callback9<R, T1, T2, T3, T4, T5, T6, T7, T8, object const &>
     : public callback_base
{
     typedef object const & T9;
     typedef R (*functor_type)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
     enum { var_start = 9 };
     
public:
     callback9(functor_type f) : f_(f) {}
     
     virtual void invoke(Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[],
          policies const &pol)
     {
          object t9 = get_var_params(interp, objc, objv, var_start, pol);
          dispatch<R>::template do_dispatch<
               T1, T2, T3, T4, T5, T6, T7, T8, T9>(
               interp, f_,
               tcl_cast<T1>::from(interp, objv[1]),
               tcl_cast<T2>::from(interp, objv[2]),
               tcl_cast<T3>::from(interp, objv[3]),
               tcl_cast<T4>::from(interp, objv[4]),
               tcl_cast<T5>::from(interp, objv[5]),
               tcl_cast<T6>::from(interp, objv[6]),
               tcl_cast<T7>::from(interp, objv[7]),
               tcl_cast<T8>::from(interp, objv[8]),
               t9);
     }

private:
     functor_type f_;
};
