// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_OPS_HH
# define MLN_CORE_OPS_HH

/*! \file mln/core/ops.hh
 * \brief Definitions of some operators.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/exact.hh>


namespace mln
{

  /*! \brief General definition of the "not equal to" operator.
   *
   * The "not equal to" operator is here defined for every milena
   * objects.  It relies on the definition of the "equal to" operator.
   * It returns "not (lhs == rhs)".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than" operator.
   *
   * The "greater than" operator is here defined for every milena
   * objects.  It relies on the definition of the "less than"
   * operator.  It returns "rhs < lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than or equal to"
   *  operator.
   *
   * The "greater than or equal to" operator is here defined for
   * every milena objects.  It relies on the definition of the "less
   * than or equal to" operator.  It returns "rhs <= lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief Default definition of the "less than or equal to"
   *  operator.
   *
   * A default version of the "less than or equal to" operator is
   * defined for every milena objects.  It relies on the definition of
   * the "less than" operator.  It returns "not (rhs < lhs)".
   *
   * \warning In the case of partial ordering between objects, this
   * operator has to be re-defined.
   */
  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return ! (exact(lhs) == exact(rhs));
  }

  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) < exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) <= exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    // if partial ordering, this operator should be re-defined!
    return ! (exact(rhs) < exact(lhs));
  }  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_OPS_HH
