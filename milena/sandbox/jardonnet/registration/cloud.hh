#ifndef CLOUD_HH
# define CLOUD_HH

# include <assert.h>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>

# include <mln/algebra/vec.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/norm/l2.hh>

//#include "quat7.hh"

namespace mln
{

  namespace geom
  {
    template <typename P>
    P center(const p_array<P>& a)
    {
      if (a.nsites() == 0)
        return P();

      algebra::vec<P::dim,float> c(literal::zero);
      for (unsigned i = 0; i < a.nsites(); ++i)
        {
          // FIXME : Ugly.
          algebra::vec<P::dim,float> ai = a[i];
          c += ai;
        }

      return algebra::to_point<P>(c / a.nsites());
    }
  }

  namespace registration
  {

    template <typename P>
    P center(const p_array<P>& a, size_t length)
    {
      algebra::vec<P::dim,float> c(literal::zero);
      for (unsigned i = 0; i < length; ++i)
        {
          // FIXME : Ugly.
          algebra::vec<P::dim,float> ai = a[i];
          c += ai;
        }

      return algebra::to_point<P>(c / length);
    }


    // FIXME : move //exist for P?
    template <typename P>
    float sqr_norm(const P& v)
    {
      float tmp = 0;
      for (unsigned i = 0; i < P::dim; i++)
        tmp += v[i] * v[i];
      return tmp;
    }

    template <typename P, typename M, typename T>
    float rms(const p_array<P>& a1,
              const T& qk,
              const M& map,
              const size_t length)
    {
      assert(length <= a1.nsites());

      float f = 0.f;
      for (size_t i = 0; i < length; ++i)
        {
          algebra::vec<3,float> a1f = qk(a1[i]);
          algebra::vec<3,float> a2f = map(algebra::to_point<P>(qk(a1[i])));
          f += norm::l2(a1f - a2f);
        }
      return f / length;
    }

    template <typename P, typename M, typename T>
    float rms(const p_array<P>& a1,
              M& map,
              const size_t length,
              const T& q1,
              const T& q2) // FIXME : transformation T can be diff ?
    {
      assert(length <= a1.nsites());

      float f = 0.f;
      for (size_t i = 0; i < length; ++i)
        {
          algebra::vec<3,float> a2f = q2(a1[i]);
          algebra::vec<3,float> a1f = map(algebra::to_point<P>(q1(a1[i])));
          f += norm::l2(a1f - a2f);
        }
      return f / length;
    }

  } // end of namespace registration
} // end of namespace mln

#endif // ndef CLOUD_HH
