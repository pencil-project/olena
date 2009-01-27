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

#ifndef MLN_REGISTRATION_MULTISCALE_HH
# define MLN_REGISTRATION_MULTISCALE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/image/lazy_image.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/registration/icp.hh>
# include <mln/fun/x2p/closest_point.hh>

namespace mln
{

  namespace registration
  {

    /*! Register point in \p c using a multiscale icp algorithm
     *
     * \param[in] cloud Image to register
     * \param[in] surface Image using to register onto
     * \param[in] q Subsampling quotient
     * \param[in] nb_it Number of registrations
     * \param[out] The rigid transformation obtained.
     *
     * \pre \p cloud has to be initialized.
     * \pre \p surface has to be initialized.
     */
    template <typename I, typename J>
    inline
    composed< rotation<I::psite::dim, float>, translation<I::Psite::dim, float> >
    multiscale(const Image<I>& cloud,
               const Image<J>& surface,
               const float q,
               const unsigned nb_it);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // FIXME: move elsewhere
      template <typename P>
      void shuffle(p_array<P>& a)
      {
        for (unsigned int i = 0; i < a.nsites(); i++)
        {
          unsigned int r = rand() % a.nsites();
          P tmp;
          tmp = a[i];
          a[i] = a[r];
          a[r] = tmp;
        }
      }

      template <typename P>
      box<P> bigger(const box<P>& a, const box<P>& b)
      {
        P pmin,pmax;

        for (unsigned i = 0; i < P::dim; i++)
        {
          pmin[i] = (a.pmin()[i] < b.pmin()[i]) ? a.pmin()[i] : b.pmin()[i];
          pmax[i] = (a.pmax()[i] > b.pmax()[i]) ? a.pmax()[i] : b.pmax()[i];
        }

        return box<P>(pmin, pmax);
      }

      template <typename P>
      inline
      box<P>            //dif
      enlarge(const box<P>& box, unsigned b)
      {
        mln::box<P> nbox(box);

        for (unsigned i = 0; i < P::dim; ++i)
        {
          nbox.pmin()[i] -= b;
          nbox.pmax()[i] += b;
        }
        return nbox;
      }

      template <typename I, typename J>
      inline
      composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
      multiscale_(const I& cloud,
                  const J& surface,
                  const float q,
                  const unsigned nb_it)
      {
        p_array<mln_psite(I)> c = convert::to< p_array<mln_psite(I)> >(cloud);
        p_array<mln_psite(J)> x = convert::to< p_array<mln_psite(J)> >(surface);

        // Shuffle cloud
        shuffle(c);

        //working box
        const box<mln_psite(I)> working_box =
          enlarge(bigger(geom::bbox(c), geom::bbox(x)), 1000); //warning 100 could be insufficient

        //make a lazy_image map via function closest_point
        fun::x2p::closest_point<mln_psite(I)> fun(x, working_box);
        const lazy_image<I, fun::x2p::closest_point<mln_psite(I)>, box2d >
          map(fun, fun.domain());

        //init rigid transform qk
        // FIXME: check qk initialisation.
        composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> > qk;

        //run registration
        for (int e = nb_it-1; e >= 0; e--)
        {
          unsigned int l = c.nsites() / std::pow(q, e);
          l = (l < 1) ? 1 : l;
          icp_subset(c, l, map, qk);
        }
        return qk;
      }

    }

    template <typename I, typename J>
    inline
    composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
    multiscale(const Image<I>& cloud,
               const Image<J>& surface,
               const float q,
               const unsigned nb_it)
    {
      trace::entering("registration::registration");

      mln_precondition(I::site::dim == 3 || I::site::dim == 2);

      composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
        qk = impl::multiscale_(exact(cloud), exact(surface), q, nb_it);

      trace::exiting("registration::registration");

      return qk;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_MULTISCALE_HH
