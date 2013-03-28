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
          typename T5, typename T6, typename T7, typename T8,
          typename T9>
struct get_callback_type_for_construct
{
     typedef callback9<C*, T1, T2, T3, T4, T5, T6, T7, T8, T9> type;
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
struct get_callback_type_for_construct<
     C, T1, T2, T3, T4, T5, T6, T7, T8, void>
{
     typedef callback8<C*, T1, T2, T3, T4, T5, T6, T7, T8> type;
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
struct get_callback_type_for_construct<
     C, T1, T2, T3, T4, T5, T6, T7, void, void>
{
     typedef callback7<C*, T1, T2, T3, T4, T5, T6, T7> type;
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
struct get_callback_type_for_construct<
     C, T1, T2, T3, T4, T5, T6, void, void, void>
{
     typedef callback6<C*, T1, T2, T3, T4, T5, T6> type;
};

template <class C, typename T1, typename T2, typename T3, typename T4,
          typename T5>
struct get_callback_type_for_construct<
     C, T1, T2, T3, T4, T5, void, void, void, void>
{
     typedef callback5<C*, T1, T2, T3, T4, T5> type;
};

template <class C, typename T1, typename T2, typename T3, typename T4>
struct get_callback_type_for_construct<
     C, T1, T2, T3, T4, void, void, void, void, void>
{
     typedef callback4<C*, T1, T2, T3, T4> type;
};

template <class C, typename T1, typename T2, typename T3>
struct get_callback_type_for_construct<
     C, T1, T2, T3, void, void, void, void, void, void>
{
     typedef callback3<C*, T1, T2, T3> type;
};

template <class C, typename T1, typename T2>
struct get_callback_type_for_construct<
     C, T1, T2, void, void, void, void, void, void, void>
{
     typedef callback2<C*, T1, T2> type;
};

template <class C, typename T1>
struct get_callback_type_for_construct<
     C, T1, void, void, void, void, void, void, void, void>
{
     typedef callback1<C*, T1> type;
};

template <class C>
struct get_callback_type_for_construct<
     C, void, void, void, void, void, void, void, void, void>
{
     typedef callback0<C*> type;
};
