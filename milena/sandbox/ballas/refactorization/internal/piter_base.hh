#ifndef INTERNAL_PITER_BASE_HH_
# define INTERNAL_PITER_BASE_HH_

# include <concept.hh>
# include <exact.hh>
# include <metal/bool.hh>
# include <metal/equal.hh>

namespace mln
{

  namespace internal
  {

    /// fwd declaration
    template <typename E, typename B>
    struct piter_base_site_cast;

    /// Psite = Site
    template <typename E>
    struct piter_base_site_cast<E, metal::true_> : public Piter<E>
    {
    };

    /// Psite != site
    template <typename E>
    struct piter_base_site_cast<E, metal::false_> : public Piter<E>
    {
      operator typename E::site() const;
    };

    /// Piter base
    template <typename E>
    struct piter_base_ :
      public piter_base_site_cast<E,
				  typename mlc_equal(typename E::site,
						     typename E::psite)::eval>
    {
      operator typename E::psite() const;

    protected:
      piter_base_();

    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    piter_base_site_cast<E, metal::false_>::operator typename E::site () const
    {
      return exact(this)->to_site();
    }


    template <typename E>
    piter_base_<E>::operator typename E::psite () const
    {
      return exact(this)->to_site();
    }


    template <typename E>
    piter_base_<E>::piter_base_()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  }

}


#endif // !INTERNAL_PITER_BASE_HH_
