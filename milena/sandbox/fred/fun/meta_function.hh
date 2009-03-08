// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_CONCEPT_META_FUNCTION_HH
# define MLN_CORE_CONCEPT_META_FUNCTION_HH

/// \file mln/core/concept/meta_function.hh
///
/// Definition of the concept of mln::Meta_Function.

# include <mln/core/concept/object.hh>
# include <mln/core/concept/function.hh>


# define mln_fun_with(F, T) \
typename F::template with< T >::ret

# define mln_fun_with_(F, T) \
F::with< T >::ret


# define mln_fun_result(F, T) \
typename F::template with< T >::ret::result


# define mln_fun_result_(F, T) \
F::with< T >::ret::result



namespace mln
{

  // Fwd decl.
  template <typename E> struct Meta_Function;

  // Meta_Function category flag type.
  template <>
  struct Meta_Function<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation of meta functions.
   *
   * The parameter \a E is the exact type.
   *
   * \see mln::doc::Meta_Function for a complete documentation of
   * this class contents.
   */
  template <typename E>
  struct Meta_Function : public Object<E>
  {
    typedef Meta_Function<void> category;
  protected:
    Meta_Function();
  };


  namespace fun
  {

    // To be specialized when some state (attributes) have to be transfered
    // from the meta-function to the function.
    // Warning: the first argument has to be an object with the exact type.
    template <typename M, typename T>
    mln_fun_with(M, T)
    unmeta(const M&, T);

    template <typename M, typename T>
    void
    unmeta(const Meta_Function<M>&, T); // Safety.

  } // end of namespace mln::fun



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Meta_Function<E>::Meta_Function()
  {
    // FIXME: Check "with" on E.
  }

  namespace fun
  {

    template <typename M, typename T>
    inline
    mln_fun_with(M, T)
    unmeta(const M&, T)
    {
      mlc_is_a(M, Meta_Function)::check();
      mln_fun_with(M, T) a;
      return a;
    }

  } // end of namespace mln::fun

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_META_FUNCTION_HH
