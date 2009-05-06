// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_LITERAL_ZERO_HH
# define MLN_LITERAL_ZERO_HH

/// \file mln/literal/zero.hh
///
/// Definition of the literal of mln::zero.
///
/// \todo Macro-ification of similar code (when no trivial conversion ops are involved).

# include <mln/core/concept/literal.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  namespace literal
  {

    /// Type of literal zero.
    struct zero_t : public Literal<zero_t>
    {
      // FIXME: (for the record) Add "friend class Literal<zero_t>;" and a protected ctor.

      template <typename T>
      operator const T () const;
    };

    /// Literal zero.
    extern const zero_t& zero;

# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    zero_t::operator const T () const
    {
      mlc_converts_to(int, T)::check();
      return 0;
    }

    const zero_t& zero = zero_t();

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_ZERO_HH
