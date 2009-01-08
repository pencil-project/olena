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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/image/line_graph_elt_neighborhood.hh
///
/// Definition of the elementary ``neighborhood'' on a line graph.

# include <mln/core/neighb.hh>
# include <mln/core/image/line_graph_elt_window.hh>


namespace mln
{

  /// Elementary neighborhood on line graph class.
  template <typename G, typename F>
  struct line_graph_elt_neighborhood
    : public neighb< line_graph_elt_window<G,F> >
  {
    typedef neighb< line_graph_elt_window<G,F> > super_;

    line_graph_elt_neighborhood();
  };


# ifndef MLN_INCLUDE_ONLY

template <typename G, typename F>
inline
line_graph_elt_neighborhood<G,F>::line_graph_elt_neighborhood()
{
}

# endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH
