// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_EXTENSION_VAL_HH
# define MLN_CORE_IMAGE_EXTENSION_VAL_HH

/*!
 * \file mln/core/image/extension_val.hh
 *
 * \brief Definition of a morpher that extends the domain of an image.
 *
 * \todo Use the 'instant' mechanism.
 * \todo Use an envelop as lvalue to test extension writing.
 */

# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Forward declaration.
  template <typename I> struct extension_val;


  namespace internal
  {

    /// Data structure for \c mln::extension_val<I>.
    template <typename I>
    struct data< extension_val<I> >
    {
      data(I& ima, const mln_value(I)& val_);

      I ima_;
      mln_value(I) val_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< extension_val<I> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::indirect value_access;

      // extended domain
      typedef trait::image::ext_domain::infinite ext_domain;
      typedef trait::image::ext_value::single    ext_value;
      typedef trait::image::ext_io::read_write   ext_io;
    };

    template <typename I, typename V>
    struct ch_value< extension_val<I>, V >
    {
      typedef mlc_converts_to(mln_value(I), V) keep_ext;
      typedef mln_ch_value(I, V) Iv;
      typedef extension_val<Iv>  Iv_ext;
      typedef mlc_if(keep_ext, Iv_ext, Iv) ret;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename I>
  class extension_val :
    public internal::image_identity< I, mln_pset(I), extension_val<I> >
  {
  public:

    /// Skeleton.
    typedef extension_val< tag::image_<I> > skeleton;


    /// Return type of read-only access.
    typedef mln_value(I) rvalue;


    /// Constructor without argument.
    extension_val();

    /// Constructor from an image \p ima and a value \p val.
    extension_val(I& ima, const mln_value(I)& val);

    /// Deferred initialization from an image \p ima and a value \p
    /// val.
    void init_(I& ima, const mln_value(I)& val);


    /// Test if \p p is valid.  It returns always true.
    // Tech note: the 'template' allows for multiple input.
    template <typename P>
    bool has(const P& p) const;


    /// Read-only access to the image value located at site \p p;
    mln_value(I) operator()(const mln_psite(I)& p) const;

    /// Read-write access to the image value located at site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);


    /// Read-only access to the value of the extension domain.
    const mln_value(I)& extension() const;

    /// Change the value of the extension domain.
    void change_extension(const mln_value(I)& val);
  };


  // init_

  template <typename I, typename J>
  void init_(tag::image_t, extension_val<I>& target, const J& model);

  template <typename V, typename I>
  void init_(tag::extension_t, V& target, const extension_val<I>& model);



# ifndef MLN_INCLUDE_ONLY

  // internal::data< extension_val<I,S> >

  namespace internal
  {

    template <typename I>
    inline
    data< extension_val<I> >::data(I& ima, const mln_value(I)& val)
      : ima_(ima),
	val_(val)
    {
    }

  } // end of namespace mln::internal

  // extension_val<I>

  template <typename I>
  inline
  extension_val<I>::extension_val()
  {
  }

  template <typename I>
  inline
  extension_val<I>::extension_val(I& ima, const mln_value(I)& val)
  {
    init_(ima, val);
  }

  template <typename I>
  inline
  void
  extension_val<I>::init_(I& ima, const mln_value(I)& val)
  {
    this->data_ = new internal::data< extension_val<I> >(ima, val);
  }

  template <typename I>
  template <typename P>
  inline
  bool
  extension_val<I>::has(const P&) const
  {
    return true;
  }

  template <typename I>
  inline
  mln_value(I)
  extension_val<I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->has_data());
    // if-else is preferred to the ternary op to allow conversions.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      return this->data_->val_;
  }

  template <typename I>
  inline
  mln_morpher_lvalue(I)
  extension_val<I>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) cpy;
    mln_precondition(this->has_data());
    // See the above comment about if-else v. ternary.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      {
	// This hack makes the extension value non-modifiable.
	cpy = this->data_->val_;
	return cpy;
      }
  }

  template <typename I>
  inline
  const mln_value(I)&
  extension_val<I>::extension() const
  {
    mln_precondition(this->has_data());
    return this->data_->val_;
  }

  template <typename I>
  inline
  void
  extension_val<I>::change_extension(const mln_value(I)& val)
  {
    mln_precondition(this->has_data());
    this->data_->val_ = val;
  }


  // init_

  template <typename I, typename J>
  void init_(tag::image_t, extension_val<I>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    mln_value(I) val;
    init_(tag::extension, val, model);
    target.init_(ima, val);
  }

  template <typename V, typename I>
  void init_(tag::extension_t, V& target, const extension_val<I>& model)
  {
    mlc_converts_to(mln_value(I), V)::check();
    target = model.extension();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_EXTENSION_VAL_HH
