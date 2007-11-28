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

#ifndef MLN_MAKE_BOX1D_HH
# define MLN_MAKE_BOX1D_HH

/*! \file mln/make/box1d.hh
 *
 * \brief Routines to construct an mln::box1d.
 */

# include <mln/core/box1d.hh>
# include <mln/make/point1d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::box1d.
     *
     * \param[in] ninds Number of inds.
     *
     * \pre \p ninds != 0 and \p ncols != 0.
     *
     * \return A 1D box.
     */
    mln::box1d box1d(unsigned ninds);


    /*! \brief Create an mln::box1d.
     *
     * \overload
     *
     * \param[in] min_ind Index of the top most ind.
     * \param[in] max_ind Index of the botton most ind.
     *
     * \pre \p max_ind >= \p min_ind.
     *
     * \return A 1D box.
     */
    mln::box1d box1d(int min_ind, int max_ind);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::box1d box1d(unsigned ninds)
    {
      mln_precondition(ninds != 0);
      mln::box1d tmp(make::point1d(0),
		     make::point1d(ninds - 1));
      return tmp;
    }

    inline
    mln::box1d box1d(int min_ind, int max_ind)
    {
      mln_precondition(max_ind >= min_ind);
      mln::box1d tmp(make::point1d(min_ind),
		     make::point1d(max_ind));
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_BOX1D_HH
