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

#ifndef MLN_CORE_CONCEPT_WINDOW_HH
# define MLN_CORE_CONCEPT_WINDOW_HH

/*! \file mln/core/concept/window.hh
 * \brief Definition of the concept of mln::Window.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>


namespace mln
{

  /*! \brief Base class for implementation classes that are windows.
   *
   * \see mln::doc::Window for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Window : public Object<E>
  {
    /*
      typedef point;
      typedef dpoint;

      typedef qiter;
      typedef fwd_qiter;
      typedef bkd_qiter;

      bool is_empty() const;
      bool is_centered() const;
      bool is_symmetric() const;

      unsigned delta() const;

      E sym_() const;
    */

  protected:
    Window();
  };


  /*! \brief Compute the symmetrical window of \p rhs.
   *
   * \relates mln::Window
   */
  template <typename W>
  W operator-(const Window<W>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Window<E>::Window()
  {
    typedef  mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;

    typedef     mln_qiter(E)     qiter;
    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;

    bool (E::*m1)() const = & E::is_empty;
    m1 = 0;
    bool (E::*m2)() const = & E::is_centered;
    m2 = 0;
    bool (E::*m3)() const = & E::is_symmetric;
    m3 = 0;
    unsigned (E::*m4)() const = & E::delta;
    m4 = 0;
    E (E::*m_)() const = & E::sym_;
    m_ = 0;
  }

  template <typename W>
  W operator-(const Window<W>& rhs)
  {
    return exact(rhs).sym_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WINDOW_HH
