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

#ifndef MLN_MAKE_W_WINDOW2D_HH
# define MLN_MAKE_W_WINDOW2D_HH

/*! \file mln/make/w_window2d.hh
 *
 * \brief Routine to create an mln::w_window in the 2D case.
 */

# include <cmath>

# include <mln/core/w_window.hh>
# include <mln/core/dpoint2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a 2D mln::w_window from an array of weights.
     *
     * \param[in] weights Array.
     *
     * \pre The array size, \c M, has to be a square of an odd integer.
     *
     * \return A 2D weighted window.
     */
    template <typename W, unsigned M>
    mln::w_window<mln::dpoint2d, W> w_window2d(W (&weights)[M]);


# ifndef MLN_INCLUDE_ONLY

    template <typename W, unsigned M>
    inline
    mln::w_window<mln::dpoint2d, W>
    w_window2d(W (&weights)[M])
    {
      int h = unsigned(std::sqrt(float(M))) / 2;
      mln_precondition((2 * h + 1) * (2 * h + 1) == M);
      mln::w_window<mln::dpoint2d, W> tmp;
      unsigned i = 0;
      for (int row = - h; row <= h; ++row)
	for (int col = - h; col <= h; ++col)
	  {
	    if (weights[i] != 0)
	      tmp.insert(weights[i], make::dpoint2d(row, col));
	    i++;
	  }
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW2D_HH
