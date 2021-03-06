// Copyright (C) 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FUNCTOR_FAST_HH
# define SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FUNCTOR_FAST_HH

/// \file
///
/// \todo Refactor with sauvola_functor.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/extension/fill.hh>

# include <scribo/binarization/internal/niblack_formula.hh>

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
#  include <scribo/binarization/internal/local_threshold_debug.hh>
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG


namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;


      template <typename I>
      struct niblack_functor_fast
      {
	niblack_functor_fast(const Image<I>& input, double K);

	// Moves in input and output images are made using "step"
	// pixels. It corresponds to the scale ratio between the input
	// image and the integral image used to give the statistics
	// values.
	enum { step = 3 };

	void init();

	// Run every 4 pixels.
	void exec(double mean, double stddev);

	void end_of_row(int);

	void finalize();


	const I input;
	mln_ch_value(I,bool) output;

	const mln_value(I)* pi;
	bool* po;

	double K_;

	scribo::binarization::internal::niblack_formula formula_;

	unsigned next_line3;
	unsigned offset1;
	unsigned offset2;
      };

#ifndef MLN_INCLUDE_ONLY

      template <typename I>
      niblack_functor_fast<I>::niblack_functor_fast(const Image<I>& input_,
						    double K)
	: input(exact(input_)),
	  pi(&input(input.domain().pmin())),
	  K_(K)
      {
	mln_precondition(exact(input).is_valid());
      }

      template <typename I>
      void
      niblack_functor_fast<I>::init()
      {
	// This initialization MUST be done here since input image
	// borders may have changed!

	// Since we iterate from a smaller image in the largest ones
	// and image at scale 1 does not always have a size which can
	// be divided by 3, some sites in the border may not be
	// processed and we must skip them.
	int more_offset = - (3 - input.ncols() % 3);
	if (more_offset == - 3)
	  more_offset = 0; // No offset needed.

	next_line3 = input.delta_offset(dpoint2d(+2,0))
	  + 2 * input.border() + more_offset;

	offset1 = input.delta_offset(dpoint2d(+1,0));
	offset2 = input.delta_offset(dpoint2d(+2,0));

	initialize(output, input);
	po = &output(output.domain().pmin());
      }

      template <typename I>
      void
      niblack_functor_fast<I>::exec(double mean, double stddev)
      {
	double th = formula_(mean, stddev, K_);

	for (int i = 0; i < step; ++i, ++po, ++pi)
	{
	  *po = (*pi <= th);
	  *(po + offset1) = (*(pi + offset1) <= th);
	  *(po + offset2) = (*(pi + offset2) <= th);
	}

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	// Store local mean
	unsigned index = pi - input.buffer();

	debug_mean.element(index) = mean * mean_debug_factor;
	debug_stddev.element(index) = stddev * stddev_debug_factor;
	debug_threshold.element(index) = th;
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

      }

      template <typename I>
      void
      niblack_functor_fast<I>::end_of_row(int)
      {
	po += next_line3;
	pi += next_line3;
      }

      template <typename I>
      void
      niblack_functor_fast<I>::finalize()
      {
      }

#endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FUNCTOR_FAST_HH
