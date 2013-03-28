//
// Copyright (C) 2004-2006, Maciej Sobczak
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//

// Note: this file is not supposed to be a stand-alone header


template <class C, typename R>
class method1<C, R, object const &> : public object_cmd_base
{
     typedef object const & T1;
     typedef R (C::*mem_type)(T1);
     typedef R (C::*cmem_type)(T1) const;
     enum { var_start = 2 };
     
public:
     method1(mem_type f) : f_(f), cmem_(false) {}
     method1(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t1 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1>(
                    interp, boost::bind(cf_, p, _1),
                    t1);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1>(
                    interp, boost::bind(f_, p, _1),
                    t1);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1>
class method2<C, R, T1, object const &> : public object_cmd_base
{
     typedef object const & T2;
     typedef R (C::*mem_type)(T1, T2);
     typedef R (C::*cmem_type)(T1, T2) const;
     enum { var_start = 3 };
     
public:
     method2(mem_type f) : f_(f), cmem_(false) {}
     method2(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t2 = get_var_params(interp, objc, objv, var_start, pol);
          
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2>(
                    interp, boost::bind(cf_, p, _1, _2),
                    tcl_cast<T1>::from(interp, objv[2]),
                    t2);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2>(
                    interp, boost::bind(f_, p, _1, _2),
                    tcl_cast<T1>::from(interp, objv[2]),
                    t2);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2>
class method3<C, R, T1, T2, object const &> : public object_cmd_base
{
     typedef object const & T3;
     typedef R (C::*mem_type)(T1, T2, T3);
     typedef R (C::*cmem_type)(T1, T2, T3) const;
     enum { var_start = 4 };
     
public:
     method3(mem_type f) : f_(f), cmem_(false) {}
     method3(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t3 = get_var_params(interp, objc, objv, var_start, pol);
          
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3>(
                    interp, boost::bind(cf_, p, _1, _2, _3),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    t3);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3>(
                    interp, boost::bind(f_, p, _1, _2, _3),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    t3);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3>
class method4<C, R, T1, T2, T3, object const &> : public object_cmd_base
{
     typedef object const & T4;
     typedef R (C::*mem_type)(T1, T2, T3, T4);
     typedef R (C::*cmem_type)(T1, T2, T3, T4) const;
     enum { var_start = 5 };
     
public:
     method4(mem_type f) : f_(f), cmem_(false) {}
     method4(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t4 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    t4);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    t4);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4>
class method5<C, R, T1, T2, T3, T4, object const &> : public object_cmd_base
{
     typedef object const & T5;
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5) const;
     enum { var_start = 6 };
     
public:
     method5(mem_type f) : f_(f), cmem_(false) {}
     method5(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t5 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4, _5),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    t5);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4, _5),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    t5);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5>
class method6<C, R, T1, T2, T3, T4, T5, object const &>
     : public object_cmd_base
{
     typedef object const & T6;
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6) const;
     enum { var_start = 7 };
     
public:
     method6(mem_type f) : f_(f), cmem_(false) {}
     method6(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t6 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4, _5, _6),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    t6);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4, _5, _6),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    t6);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6>
class method7<C, R, T1, T2, T3, T4, T5, T6, object const &>
     : public object_cmd_base
{
     typedef object const & T7;
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7) const;
     enum { var_start = 8 };
     
public:
     method7(mem_type f) : f_(f), cmem_(false) {}
     method7(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t7 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6, T7>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4, _5, _6, _7),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    t7);
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6, T7>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4, _5, _6, _7),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    t7);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6, typename T7>
class method8<C, R, T1, T2, T3, T4, T5, T6, T7, object const &>
     : public object_cmd_base
{
     typedef object const & T8;
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7, T8);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7, T8) const;
     enum { var_start = 9 };
     
public:
     method8(mem_type f) : f_(f), cmem_(false) {}
     method8(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t8 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<
                    T1, T2, T3, T4, T5, T6, T7, T8>(
                    interp, boost::bind(cf_, p,
                         _1, _2, _3, _4, _5, _6, _7, _8),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    tcl_cast<T7>::from(interp, objv[8]),
                    t8);
          }
          else
          {
               dispatch<R>::template do_dispatch<
                    T1, T2, T3, T4, T5, T6, T7, T8>(
                    interp, boost::bind(f_, p,
                         _1, _2, _3, _4, _5, _6, _7, _8),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    tcl_cast<T7>::from(interp, objv[8]),
                    t8);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6, typename T7, typename T8>
class method9<C, R, T1, T2, T3, T4, T5, T6, T7, T8, object const &>
     : public object_cmd_base
{
     typedef object const & T9;
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7, T8, T9) const;
     enum { var_start = 10 };
     
public:
     method9(mem_type f) : f_(f), cmem_(false) {}
     method9(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &pol)
     {
          C *p = static_cast<C*>(pv);

          object t9 = get_var_params(interp, objc, objv, var_start, pol);

          if (cmem_)
          {
               dispatch<R>::template do_dispatch<
                    T1, T2, T3, T4, T5, T6, T7, T8, T9>(
                    interp, boost::bind(cf_, p,
                         _1, _2, _3, _4, _5, _6, _7, _8, _9),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    tcl_cast<T7>::from(interp, objv[8]),
                    tcl_cast<T8>::from(interp, objv[9]),
                    t9);
          }
          else
          {
               dispatch<R>::template do_dispatch<
                    T1, T2, T3, T4, T5, T6, T7, T8, T9>(
                    interp, boost::bind(f_, p,
                         _1, _2, _3, _4, _5, _6, _7, _8, _9),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]),
                    tcl_cast<T7>::from(interp, objv[8]),
                    tcl_cast<T8>::from(interp, objv[9]),
                    t9);
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};
