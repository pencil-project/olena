// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_ALIAS_DPOINT3D_HH
# define MLN_CORE_ALIAS_DPOINT3D_HH

/// \file mln/core/alias/dpoint3d.hh
///
/// Definition of the mln::dpoint3d alias and of its
/// construction routine.

# include <mln/core/dpoint.hh>
# include <mln/core/grids.hh>
# include <mln/core/def/coord.hh>

namespace mln
{

  /// Type alias for a delta-point defined on the 3D square
  /// grid with integer coordinates.
  typedef dpoint<mln::grid::cube, def::coord> dpoint3d;


} // end of namespace mln


# include <mln/core/alias/point3d.hh>


namespace mln
{

  /// Definition of a shortcut for delta point in 3d.
  /// \{

  extern const dpoint3d sagittal_dec;
  extern const dpoint3d sagittal_inc;
  extern const dpoint3d axial_dec;
  extern const dpoint3d axial_inc;
  extern const dpoint3d coronal_dec;
  extern const dpoint3d coronal_inc;

  /// \}


# ifndef MLN_INCLUDE_ONLY

  const dpoint3d sagittal_dec = dpoint3d( 0,  0, -1);
  const dpoint3d sagittal_inc = dpoint3d( 0,  0, +1);
  const dpoint3d axial_dec    = dpoint3d( 0, -1,  0);
  const dpoint3d axial_inc    = dpoint3d( 0, +1,  0);
  const dpoint3d coronal_dec  = dpoint3d(-1,  0,  0);
  const dpoint3d coronal_inc  = dpoint3d(+1,  0,  0);

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_DPOINT3D_HH
