// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MAKE_W_WINDOW3D_HH
# define MLN_MAKE_W_WINDOW3D_HH

/*! \file
 *
 * \brief Routine to create an mln::w_window in the 3D case.
 */

# include <cmath>

# include <mln/core/w_window.hh>
# include <mln/core/alias/dpoint3d.hh>

#include <iostream>

namespace mln
{

  namespace make
  {

    /*! \brief Create a 3D mln::w_window from an array of weights.
     *
     * \param[in] weights Array.
     *
     * \pre The array size, \c M, has to be a cube of an odd integer.
     *
     * \return A 3D weighted window.
     */
    template <typename W, unsigned M>
    mln::w_window<mln::dpoint3d, W> w_window3d(W (&weights)[M]);


# ifndef MLN_INCLUDE_ONLY

    template <typename W, unsigned M>
    inline
    mln::w_window<mln::dpoint3d, W>
    w_window3d(W (&weights)[M])
    {
      int h = unsigned(std::pow(float(M), float(1. / 3))) / 2;
      mln_precondition((2 * h + 1) * (2 * h + 1) * (2 * h + 1) == M);
      mln::w_window<mln::dpoint3d, W> tmp;
      unsigned i = 0;
      for (int sli = - h; sli <= h; ++sli)
	for (int row = - h; row <= h; ++row)
	  for (int col = - h; col <= h; ++col)
	  {
	    const W& cur = weights[i];

	    if (cur != 0)
	      tmp.insert(cur, dpoint3d(sli, row, col));
	    i++;
	  }
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW3D_HH
