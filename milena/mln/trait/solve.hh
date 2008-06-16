// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_TRAIT_SOLVE_HH
# define MLN_TRAIT_SOLVE_HH

/*!
 * \file   mln/trait/solve.hh
 *
 * \brief  FIXME
 *
 *
 */

# include <mln/core/category.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/ret.hh>


// FIXME: Just for the record (use it...)

#  ifndef MLN_DEBUG_TRAITS 
#  endif // ! MLN_DEBUG_TRAITS


# define mln_trait_unary(Name, T)  typename mln::trait::solve_unary< Name, T >::ret
# define mln_trait_unary_(Name, T)          mln::trait::solve_unary< Name, T >::ret

# define mln_trait_binary(Name, T1, T2)  typename mln::trait::solve_binary< Name, T1, T2 >::ret
# define mln_trait_binary_(Name, T1, T2)          mln::trait::solve_binary< Name, T1, T2 >::ret



namespace mln
{

  namespace trait
  {

    /// Flag type for a not found trait.
    struct not_found {};

    /// Flag type for an undefined trait.
    struct undefined {};

    /// Flag type for a trait that is multiply undefined.
    struct multiply_defined {};



    // Utility meta-function: from a category (meta) and a type, get the super category.

    namespace internal
    {

      template < template <class> class Category, typename T,
		 typename Super_Category >
      struct helper_super_category_;

      template < template <class> class Category, typename T,
		 template <class> class Super_Category >
      struct helper_super_category_< Category, T,
				     Super_Category<void> >
      {
	typedef Super_Category<void> ret; // One super category: keep it.
      };

      template < template <class> class Category, typename T >
      struct helper_super_category_< Category, T,
				     void* > // Meaning: several super categories exist, depending on T.
      {
	typedef typename mln::category< T >::super ret; // Specific call depending on T.
      };


      template < template <class> class Category, typename T >
      struct super_category_ // Entry.
      {
	typedef typename helper_super_category_< Category, T,
						 typename Category<void>::super >::ret ret;
      };

    } // end of namespace mln::trait::internal



    // Unary case.


    template < template <class> class Name,
	       typename T >
    struct set_precise_unary_
    {
      typedef undefined ret;
    };


    template < template <class> class Name,
	       template <class> class Category_T, typename T >
    struct set_unary_
    {
      typedef undefined ret;
    };

    template < template <class> class Name,
	       typename T >
    struct set_unary_< Name, Unknown, T > // Blocker; top of inheritance.
    {
      typedef not_found ret;
    };




    // Binary case.


    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct set_precise_binary_
    {
      typedef undefined ret;
    };

    
    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_
    {
      typedef undefined ret;
    };

    template < template <class, class> class Name,
	       typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_< Name, Unknown, L, Category_R, R > // Left blocker.
    {
      typedef not_found ret;
    };

    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       typename R >
    struct set_binary_< Name, Category_L, L, Unknown, R > // Right blocker.
    {
      typedef not_found ret;
    };

    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct set_binary_< Name, Unknown, L, Unknown, R > // Blocker.
    {
      typedef not_found ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/solve_unary.hh>
# include <mln/trait/solve_binary.hh>


#endif // ! MLN_TRAIT_SOLVE_HH
