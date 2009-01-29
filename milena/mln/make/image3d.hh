// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_IMAGE3D_HH
# define MLN_MAKE_IMAGE3D_HH

/// \file mln/make/image3d.hh
///
/// Routine to create a 3D image from an array of 2D images.
///
/// \todo Think about removing make::image3d since it is redundant
/// with convert::to and convert::from_to.

# include <mln/core/image/image3d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/slice_image.hh>
# include <mln/data/paste.hh>
# include <mln/util/array.hh>



namespace mln
{

  namespace make
  {

    /// Create an image3d from an array of 2D images.
    ///
    template <typename I>
    mln::image3d<mln_value(I)>
    image3d(const util::array<I>& ima);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln::image3d<mln_value(I)>
    image3d(const util::array<I>& ima)
    {
      mlc_is_a(mln_pset(I), Box)::check();
      mln_precondition(! ima.is_empty());

      def::coord n_slices = ima.nelements();
      mln::box2d b = ima[0].domain();
      mln::box3d b_ = make::box3d(0,            b.pmin().row(), b.pmin().col(),
				  n_slices - 1, b.pmax().row(), b.pmax().col());
      mln::image3d<mln_value(I)> output(b_);
      for (def::coord sli = 0; sli < n_slices; ++sli)
	{
	  mln_assertion(ima[sli].domain() == b);
	  data::paste(ima[sli], slice(output, sli).rw());
	}
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_IMAGE3D_HH