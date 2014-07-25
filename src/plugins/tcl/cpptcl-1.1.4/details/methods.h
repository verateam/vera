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
class method0 : public object_cmd_base
{
     typedef R (C::*mem_type)();
     typedef R (C::*cmem_type)() const;
     
public:
     method0(mem_type f) : f_(f), cmem_(false) {}
     method0(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int, Tcl_Obj * CONST [], policies const &)
     {
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::do_dispatch(interp, boost::bind(cf_, p));
          }
          else
          {
               dispatch<R>::do_dispatch(interp, boost::bind(f_, p));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1>
class method1 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1);
     typedef R (C::*cmem_type)(T1) const;
     
public:
     method1(mem_type f) : f_(f), cmem_(false) {}
     method1(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 3);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1>(
                    interp, boost::bind(cf_, p, _1),
                    tcl_cast<T1>::from(interp, objv[2]));
          }
          else
          {
               dispatch<R>::template do_dispatch<T1>(
                    interp, boost::bind(f_, p, _1),
                    tcl_cast<T1>::from(interp, objv[2]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2>
class method2 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2);
     typedef R (C::*cmem_type)(T1, T2) const;
     
public:
     method2(mem_type f) : f_(f), cmem_(false) {}
     method2(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 4);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2>(
                    interp, boost::bind(cf_, p, _1, _2),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]));
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2>(
                    interp, boost::bind(f_, p, _1, _2),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3>
class method3 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3);
     typedef R (C::*cmem_type)(T1, T2, T3) const;
     
public:
     method3(mem_type f) : f_(f), cmem_(false) {}
     method3(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 5);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3>(
                    interp, boost::bind(cf_, p, _1, _2, _3),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]));
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3>(
                    interp, boost::bind(f_, p, _1, _2, _3),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4>
class method4 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4);
     typedef R (C::*cmem_type)(T1, T2, T3, T4) const;
     
public:
     method4(mem_type f) : f_(f), cmem_(false) {}
     method4(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 6);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]));
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5>
class method5 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5) const;
     
public:
     method5(mem_type f) : f_(f), cmem_(false) {}
     method5(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 7);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4, _5),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]));
          }
          else
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5>(
                    interp, boost::bind(f_, p, _1, _2, _3, _4, _5),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6>
class method6 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6) const;
     
public:
     method6(mem_type f) : f_(f), cmem_(false) {}
     method6(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 8);
          
          C *p = static_cast<C*>(pv);
          if (cmem_)
          {
               dispatch<R>::template do_dispatch<T1, T2, T3, T4, T5, T6>(
                    interp, boost::bind(cf_, p, _1, _2, _3, _4, _5, _6),
                    tcl_cast<T1>::from(interp, objv[2]),
                    tcl_cast<T2>::from(interp, objv[3]),
                    tcl_cast<T3>::from(interp, objv[4]),
                    tcl_cast<T4>::from(interp, objv[5]),
                    tcl_cast<T5>::from(interp, objv[6]),
                    tcl_cast<T6>::from(interp, objv[7]));
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
                    tcl_cast<T6>::from(interp, objv[7]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6, typename T7>
class method7 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7) const;
     
public:
     method7(mem_type f) : f_(f), cmem_(false) {}
     method7(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 9);
          
          C *p = static_cast<C*>(pv);
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
                    tcl_cast<T7>::from(interp, objv[8]));
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
                    tcl_cast<T7>::from(interp, objv[8]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6, typename T7, typename T8>
class method8 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7, T8);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7, T8) const;
     
public:
     method8(mem_type f) : f_(f), cmem_(false) {}
     method8(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 10);
          
          C *p = static_cast<C*>(pv);
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
                    tcl_cast<T8>::from(interp, objv[9]));
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
                    tcl_cast<T8>::from(interp, objv[9]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};

template <class C, typename R, typename T1, typename T2, typename T3,
     typename T4, typename T5, typename T6, typename T7, typename T8,
     typename T9>
class method9 : public object_cmd_base
{
     typedef R (C::*mem_type)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
     typedef R (C::*cmem_type)(T1, T2, T3, T4, T5, T6, T7, T8, T9) const;
     
public:
     method9(mem_type f) : f_(f), cmem_(false) {}
     method9(cmem_type f) : cf_(f), cmem_(true) {}
     
     virtual void invoke(void *pv, Tcl_Interp *interp,
          int objc, Tcl_Obj * CONST objv[], policies const &)
     {
          check_params_no(objc, 11);
          
          C *p = static_cast<C*>(pv);
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
                    tcl_cast<T9>::from(interp, objv[10]));
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
                    tcl_cast<T9>::from(interp, objv[10]));
          }
     }

private:
     mem_type f_;
     cmem_type cf_;
     bool cmem_;
};
