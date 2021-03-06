// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_MORPHERS_LAZY_RECORDER_HH
# define APPS_MORPHERS_LAZY_RECORDER_HH

/// \file
/// \brief Morpher recording every change in the morphed image in a
/// lazy fashion, storing each write operation (i.e., the (site,
/// value) pairs).
///
/// The data recorded by this morpher can thus be used to ``replay''
/// the set of changes applied to an image.
///
/// \todo Split and move this into the library?

#include <sstream>
#include <iomanip>

#include <string>

#include <mln/trait/ch_value.hh>

#include <mln/core/image/imorph/decorated_image.hh>

#include <mln/value/rgb8.hh>

#include <mln/core/routine/duplicate.hh>

#include <mln/io/ppm/save.hh>

#include "apps/data.hh"


// Forward declaration.
template <typename I> struct lazy_recorder;

/* FIXME: mln::decorated_image lacks a proper definition of
   properties! (see mln/core/image/imorph/decorated_image.hh).  We use
   the following (minimal) set of properties as a workaround for the
   lazy_recorder decoration.  */
namespace mln
{
  namespace trait
  {

    template <typename I>
    struct image_< decorated_image< I, lazy_recorder<I> > >
      : default_image_morpher< I,
			       mln_value(I),
			       decorated_image< I, lazy_recorder<I> > >
    {
      typedef trait::image::category::identity_morpher category;

      // Prevent fast processing of images requiring a specific
      // interface that we are unable to retrieve now.
      typedef trait::image::value_storage::disrupted   value_storage;
    };

  } // end of namespace mln::trait

} // end of namespace mln


// ``Lazy'' recorder.
template <typename I>
struct lazy_recorder
{
  lazy_recorder()
  {
  }

  lazy_recorder(mln::Image<I>& ima)
    : initial(mln::duplicate(ima))
  {
  }

  void reading(const I&, const mln_psite(I)&) const
  {
    // N/A.
  }

  void writing(I&, const mln_psite(I)& p, const mln_value(I)& v)
  {
    sequence.push_back(std::make_pair(p, v));
  }

  /// The initial image.
  mln_concrete(I) initial;
  /// The sequence of changes (list of (psite, value) pairs).
  // Make it a tracked_ptr to avoid costly copy operations?
  std::vector< std::pair<mln_psite(I), mln_value(I)> > sequence;
};

/* Skeleton of an image decorated with a lazy_recorder.

   Initialy, I (Roland) wanted to add this to mln/trait/ch_value.hh:

     template < template <class, class> class M, typename I, typename D,
		typename V >
     struct ch_value_<  M< tag::image_<I>, tag::data_<D> >,  V  >
     {
       typedef M< mln_ch_value(I, V), D > ret;
     };

   However, this would not work in the case of the lazy_recorder since the
   type D of the data attached to the image (of type I) has to be
   changed as well.  Indeed the initial decoration contains a sequence
   of images of type I, which should be changed into a sequence of
   images of type mln_ch_value(I, V).

   There are several option to improve this.  One is to create a
   ch_value trait for data/decorations such as `lazy_recorder<I>'.  Another
   one is to refine the skeleton of decorated_image<I, D> to have it
   convey the type the data stored in the decoration, e.g, changing

     typedef decorated_image< tag::image_<I>, tag::data_<D> > skeleton;

   into something like

     typedef decorated_image< tag::image_<I>, tag::data_<D, V> > skeleton;

   but this seems overly complicated.

   The workaround chosen here is very local, and address the very
   specific case of decorated_image< I, lazy_recorder<I> >.  */

namespace mln
{
  namespace trait
  {
    namespace impl
    {
      template < typename I, typename V >
      struct ch_value_< decorated_image< tag::image_<I>,
					 tag::data_< lazy_recorder<I> > >,
			V >
      {
	typedef decorated_image< mln_ch_value(I, V),
				 lazy_recorder< mln_ch_value(I, V) > > ret;
      };
    } // end namespace mln::trait::impl

  } // end namespace mln::trait

} // end namespace mln

// Helper.
template <typename I>
inline
mln::decorated_image< I, lazy_recorder<I> >
lazy_record(mln::Image<I>& ima)
{
  return mln::decorate(ima, lazy_recorder<I>(ima));
}


// I/O.
namespace ppm
{
  template <typename I>
  inline
  void
  save(const mln::decorated_image< I, lazy_recorder<I> >& rec,
       const std::string& prefix)
  {
    mln_concrete(I) frame = mln::duplicate(rec.decoration().initial);
    for (size_t i = 0; i < rec.decoration().sequence.size(); ++i)
      {
	std::stringstream s;
	s << std::setfill ('0') << std::setw (6) << i;
	mln::io::ppm::save(frame, prefix + s.str() + ".ppm");
	// Apply the I-th change to the next frame.
	//
	// Changes are applied after the I-th image has been written,
	// to mimic the behavior of the original recorder morpher (see
	// recorder.hh).
	frame(rec.decoration().sequence[i].first) =
	  rec.decoration().sequence[i].second;
      }
  }
}

#endif // ! APPS_MORPHERS_LAZY_RECORDER_HH
