// Copyright (C) 2007.2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_MONO_OBASED_RLE_IMAGE_HH
# define MLN_CORE_IMAGE_MONO_OBASED_RLE_IMAGE_HH

/// \file mln/core/image/mono_obased_rle_image.hh
///
/// Definition of an image with rle encoding.

# include <mln/core/internal/run_image.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/core/site_set/p_set_of.hh>
# include <mln/core/site_set/box.hh>
# include <mln/value/set.hh>
# include <mln/core/image/mono_rle_image.hh>
# include <vector>

namespace mln
{

  // Forward declaration.
  template <typename P, typename T> struct mono_obased_rle_image;


  namespace internal
  {

    /// Data structure for \c mln::mono_obased_rle_image<P,T>.
    template <typename P, typename T>
    struct data< mono_obased_rle_image<P,T> >
    {
      data(const std::set<T>& values);

      /// Objects.
      std::vector< mono_rle_image<P, T> > ima_;

      /// Image value.
      std::vector<T> values_;

      /// domain of the image
      p_set_of< p_run<P> > domain_;

      /// Return the size of the data in memory.
      unsigned memory_size() const;

      /// Finalize the domain (internal use).
      void finalize();

    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename T>
    struct image_< mono_obased_rle_image<P,T> > : default_image_< T, mono_obased_rle_image<P,T> >
    {
      typedef trait::image::category::primary category;

      // FIXME: update with new properties.
//      typedef trait::image::access::browsing   access;
      // FIXME: Put the right dimension.
//      typedef trait::image::space::two_d     space;
//      typedef trait::image::size::regular    size;
//      typedef trait::image::support::aligned support;
//
//      typedef trait::image::border::none     border;
//      typedef trait::image::data::linear     data;
//      typedef trait::image::io::read_only    io;
//      typedef trait::image::speed::slow      speed;
    };

  } // end of namespace mln::trait


  /*! RLE image with objects based on mln::mono_rle_image.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class mono_obased_rle_image : public internal::run_image_< T, P, mono_obased_rle_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef p_set_of< p_run<P> > pset;
    typedef mln_psite(pset) psite;


    /// Skeleton.
    typedef mono_obased_rle_image< tag::psite_<P>, tag::value_<T> > skeleton;

    /// Constructor.
    mono_obased_rle_image(const std::set<T>& values);

    /// Initialize an empty image.
    void init_(const std::set<T>& values);

    /// Add a new range to the image.
    void insert(const p_run<P>& pr, T value);

    /// Read-only access to the image value located at point \p p.
    const T operator()(const mln_psite(pset)& site) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const mln_psite(pset)& site);

    /// Test if this image has been initialized.
    bool is_valid() const;

    /// Give the definition domain.
    const pset& domain() const;

    /// Give the index vector of the i-th object.
    const mono_rle_image<P, T>& object(unsigned i) const;

  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< mono_obased_rle_image<P,T> >

    template <typename P, typename T>
    inline
    data< mono_obased_rle_image<P,T> >::data(const std::set<T>& values)
      : ima_(values.begin(), values.end())
    {
    }

    template <typename P, typename T>
    inline
    unsigned
    data< mono_obased_rle_image<P,T> >::memory_size() const
    {
      return domain_.memory_size() * 2 + sizeof(T) * (values_.size() + ima_.size());
    }

    template <typename P, typename T>
    inline
    void
    data< mono_obased_rle_image<P,T> >::finalize()
    {
      domain_.finalize();
      for (typename std::vector< mono_rle_image<P, T> >::iterator it = ima_.begin();
	   it != ima_.end(); ++it)
	it->finalize();
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  inline
  mono_obased_rle_image<P, T>::mono_obased_rle_image(const std::set<T>& values)
  {
    init_(values);
  }

  template <typename P, typename T>
  inline
  void
  mono_obased_rle_image<P, T>::init_(const std::set<T>& values)
  {
    this->data_ = new internal::data< mono_obased_rle_image<P,T> >(values);
  }

  template <typename P, typename T>
  inline
  bool
  mono_obased_rle_image<P, T>::is_valid() const
  {
    return this->data_->ima_.size() != 0;
  }

  template <typename P, typename T>
  inline
  void
  mono_obased_rle_image<P, T>::insert(const p_run<P>& pr, T value)
  {
    mln_assertion(this->data_->values_.size() == 0 || this->data_->domain_.nsites() == 0 ||
		  pr[0] > this->data_->domain_[this->data_->domain_.nsites() - 1].start());
    this->data_->domain_.insert(pr);
    this->data_->values_.push_back(value);
    unsigned i;
    for (i = 0; i < this->data_->ima_.size()
	   && this->data_->ima_[i].get_value() != value; ++i)
      ;
    mln_assertion(i != this->data_->ima_.size());
    this->data_->ima_[i].insert(pr);
  }

  template <typename P, typename T>
  inline
  const T
  mono_obased_rle_image<P, T>::operator() (const mln_psite(pset)& site) const
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  T&
  mono_obased_rle_image<P, T>::operator() (const mln_psite(pset)& site)
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  const typename mono_obased_rle_image<P, T>::pset&
  mono_obased_rle_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

  template <typename P, typename T>
  inline
  const mono_rle_image<P, T>&
  mono_obased_rle_image<P, T>::object(unsigned i) const
  {
    mln_assertion(i < this->data_->ima_.size());
    return this->data_->ima_[i];
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_MONO_OBASED_RLE_IMAGE_HH
