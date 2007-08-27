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

#ifndef MLN_LINEAR_LOG_HH
# define MLN_LINEAR_LOG_HH

/*! \file mln/linear/log.hh
 *
 * \brief Laplacian of Gaussian.
 */

# include <mln/linear/convolve.hh>
# include <mln/make/w_window2d.hh>



namespace mln
{

  namespace linear
  {

    template <typename I, typename O>
    void LoG_5x5(const Image<I>& input, Image<O>& output);

    template <typename I, typename O>
    void LoG_7x7(const Image<I>& input, Image<O>& output);

    template <typename I, typename O>
    void LoG_13x13(const Image<I>& input, Image<O>& output);

    template <typename I, typename O>
    void LoG_17x17(const Image<I>& input, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    // LoG_5x5  (Cf. Sonka et al., pages 85-86)
    // This is also a "mexican hat".

    template <typename I, typename O>
    void LoG_5x5(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      int ws[] = { +0,  0, -1,  0,  0,
		   +0, -1, -2, -1,  0,
		   -1, -2, 16, -2, -1,
		   +0, -1, -2, -1,  0,
		   +0,  0, -1,  0,  0 };
      convolve(input, make::w_window2d(ws), output);
    }

    // LoG 7x7  (Cf. Russ, p. 250)

    template <typename I, typename O>
    void LoG_7x7(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      int ws[] = { +0,  0, -1, -1, -1,  0,  0, 
		   +0, -1, -3, -3, -3, -1,  0,
		   -1, -3,  0,  7,  0, -3, -1,
		   -1, -3,  7, 24,  7, -3, -1,
		   -1, -3,  0,  7,  0, -3, -1,
		   +0, -1, -3, -3, -3, -1,  0,
		   +0,  0, -1, -1, -1,  0,  0 };
      convolve(input, make::w_window2d(ws), output);
    }

    // LoG 13x13  (Cf. Russ, p. 250)

    template <typename I, typename O>
    void LoG_13x13(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      int ws[] = { +0,  0,  0,  0,  0, -1, -1, -1,  0,  0,  0,  0,  0,
		   +0,  0,  0, -1, -1, -2, -2, -2, -1, -1,  0,  0,  0,
		   +0,  0, -2, -2, -3, -3, -4, -3, -3, -2, -2,  0,  0,
		   +0, -1, -2, -3, -3, -3, -2, -3, -3, -3, -2, -1,  0,
		   +0, -1, -3, -3, -1,  4,  6,  4, -1, -3, -3, -1,  0,
		   -1, -2, -3, -3,  4, 14, 19, 14,  4, -3, -3, -2, -1,
		   -1, -2, -4, -2,  6, 19, 24, 19,  6, -2, -4, -2, -1,
		   -1, -2, -3, -3,  4, 14, 19, 14,  4, -3, -3, -2, -1,
		   +0, -1, -3, -3, -1,  4,  6,  4, -1, -3, -3, -1,  0,
		   +0, -1, -2, -3, -3, -3, -2, -3, -3, -3, -2, -1,  0,
		   +0,  0, -2, -2, -3, -3, -4, -3, -3, -2, -2,  0,  0,
		   +0,  0,  0, -1, -1, -2, -2, -2, -1, -1,  0,  0,  0,
		   +0,  0,  0,  0,  0, -1, -1, -1,  0,  0,  0,  0,  0 };
      convolve(input, make::w_window2d(ws), output);
    }


    // LoG 17x17  (Cf. Sonka et al., pages 85-86)

    template <typename I, typename O>
    void LoG_17x17(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      int ws[] = { +0, 0, 0, 0, 0, 0,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0,
		   +0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0,
		   +0, 0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1, 0, 0,
		   +0, 0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1, 0, 0,
		   +0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1, 0,
		   +0,-1,-2,-3,-3,-3, 0, 2, 4, 2, 0,-3,-3,-3,-2,-1, 0,
		   -1,-1,-3,-3,-3, 0, 4,10,12,10, 4, 0,-3,-3,-3,-1,-1,
		   -1,-1,-3,-3,-2, 2,10,18,21,18,10, 2,-2,-3,-3,-1,-1,
		   -1,-1,-3,-3,-3, 4,12,21,24,21,12, 4,-3,-3,-3,-1,-1,
		   -1,-1,-3,-3,-2, 2,10,18,21,18,10, 2,-2,-3,-3,-1,-1,
		   -1,-1,-3,-3,-3, 0, 4,10,12,10, 4, 0,-3,-3,-3,-1,-1,
		   +0,-1,-2,-3,-3,-3, 0, 2, 4, 2, 0,-3,-3,-3,-2,-1, 0,
		   +0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1, 0,
		   +0, 0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1, 0, 0,
		   +0, 0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1, 0, 0,
		   +0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0,
		   +0, 0, 0, 0, 0, 0,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0 };
      convolve(input, make::w_window2d(ws), output);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_LOG_HH
