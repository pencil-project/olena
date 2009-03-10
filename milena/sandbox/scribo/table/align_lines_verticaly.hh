// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH
# define SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH

/// \file scribo/table/align_lines_verticaly.hh
///
/// Align line bounding boxes verticaly.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/min_ncol.hh>
# include <mln/geom/max_ncol.hh>

# include <mln/util/array.hh>

# include <scribo/table/internal/align_lines.hh>



namespace scribo
{

  namespace table
  {

    /// Align line bounding boxes verticaly.
    ///
    /// \param[in]	input	     Image from which the line bboxes are
    ///				     extracted from.
    /// \param[in, out] lines_bboxes vertical lines bounding boxes.
    ///
    /// \return A list of the resulting aligned cols. Each integer is actually
    ///		a col number.
    template <typename I>
    util::array<int>
    align_lines_verticaly(const Image<I>& input,
			  util::array<box<mln_site(I)> >& lines_bboxes);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    util::array<int>
    align_lines_verticaly(const Image<I>& input,
			  util::array<box<mln_site(I)> >& lines_bboxes)
    {
      trace::entering("scribo::table::align_lines_horizontaly");

      mln_precondition(exact(input).is_valid());
      util::array<int> res = internal::align_lines(geom::ncols(input),
						   geom::min_col(input),
						   geom::max_col(input),
						   lines_bboxes, 1);

      trace::exiting("scribo::table::align_lines_horizontaly");
      return res;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH