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

#ifndef MLN_ACCU_MIN_MAX_HH
# define MLN_ACCU_MIN_MAX_HH

/*! \file mln/accu/min_max.hh
 *
 * \brief Define an accumulator that computes a min and a max.
 *
 * \todo A macro to create such accumulators.
 */

# include <utility>

# include <mln/accu/pair.hh>
# include <mln/accu/min.hh>
# include <mln/accu/max.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {

    /*! \brief Generic min and max accumulator class.
     *
     * The parameter \c V is the type of values.
     */
    template <typename V>
    struct min_max_ : public pair_< min_<V>, max_<V> >
    {
    };


    template <typename I> struct min_max_< util::pix<I> >;


    namespace meta
    {

      // FIXME: Doc!
      typedef pair<min,max> min_max;

    } // end of namespace mln::accu::meta


  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MIN_MAX_HH
