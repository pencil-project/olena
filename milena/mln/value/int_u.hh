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

#ifndef MLN_VALUE_INT_U_HH
# define MLN_VALUE_INT_U_HH

/*! \file mln/value/int_u.hh
 *
 * \brief Define a generic class for unsigned integers.
 */

# include <mln/value/ops.hh>

# include <mln/metal/math/pow.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/value/concept/integer.hh>
# include <mln/trait/value_.hh>
# include <mln/trait/all.hh> // FIXME!
# include <mln/trait/value_.hh>
# include <mln/debug/format.hh>



namespace mln
{

  namespace value
  {
    /// \{ Fwd decls.
    template <unsigned n> struct int_u;
    /// \}
  }

  namespace literal
  {
    /// \{ Fwd decls.
    struct zero_t;
    struct one_t;
    /// \}
  }


  namespace trait
  {

    template <unsigned n>
    struct set_precise_unary_< op::uminus, mln::value::int_u<n> >
    {
      typedef int ret;
    };


    template <unsigned n>
    struct value_< mln::value::int_u<n> >
    {
    private:
      typedef mln::value::int_u<n> self_;
      typedef typename mln::value::internal::encoding_unsigned_<n>::ret enc_;
    public:

      enum {
	nbits = n,
	card  = mln_value_card_from_(n)
      };

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::data      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ min() { return 0; }
      static const self_ max() { return mlc_pow_int(2, n) - 1; }
      static const self_ epsilon() { return 0; }

      typedef float sum;
    };

  } // end of namespace mln::trait



  namespace value
  {

    /*! \brief Unsigned integer value class.
     *
     * The parameter is \c n the number of encoding bits.
     */
    template <unsigned n>
    struct int_u
      :
      public Integer< int_u<n> >,

      public internal::value_like_< unsigned,    // Equivalent.
				    typename internal::encoding_unsigned_<n>::ret, // Enc.
				    int,         // Interoperation.
				    int_u<n> >   // Exact.
    {
    protected:
      /// Encoding associated type.
      typedef typename internal::encoding_unsigned_<n>::ret enc_;

    public:

      /// Constructor without argument.
      int_u();

      /// Constructor from an integer.
      int_u(int i);

      /// \{ Constructors/assignments with literals.
      int_u(const literal::zero_t&);
      int_u& operator=(const literal::zero_t&);
      int_u(const literal::one_t&);
      int_u& operator=(const literal::one_t&);
      /// \}

      /// Conversion to an unsigned integer.
      operator unsigned() const;

      /// Unary operator minus.
      int operator-() const;

      /// Assignment from an integer.
      int_u<n>& operator=(int i);
    };


    // Safety.
    template <> struct int_u<0>;
    template <> struct int_u<1>;



    /*! \brief Print an unsigned integer \p i into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i An unsigned integer.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i);


    // FIXME: Doc!
    template <unsigned n>
    std::istream& operator>>(std::istream& istr, int_u<n>& i);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    inline
    int_u<n>::int_u()
    {
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = enc_(i);
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const literal::zero_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const literal::one_t&)
    {
      this->v_ = 1;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::operator unsigned() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    int
    int_u<n>::operator-() const
    {
      return - int(this->v_);
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = i;
      return *this;
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i)
    {
      // FIXME: This code could be factored for almost every Value<*>...
      return ostr << debug::format(i.to_equiv()); // FIXME: is to_equiv OK?
    }

    template <unsigned n>
    inline
    std::istream& operator>>(std::istream& istr, int_u<n>& i)
    {
      return istr >> i.handle_();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INT_U_HH
