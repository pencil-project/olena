// Copyright (C) 2009 EPITA Research and Development Laboratory
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

/// \file scribo/src/dmap.cc
///
/// Compute a distance map and an influence zone image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/wrap.hh>

#include <mln/transform/distance_and_influence_zone_geodesic.hh>

#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm dmap.pgm iz.ppm"
	    << std::endl
	    << "  Compute a distance map and an influence zone image."
	    << std::endl;
  return 1;
}


int main(int argc, char *argv[])
{
  using namespace mln;

  using value::label_16;
  using value::rgb8;

  if (argc != 4)
    return usage(argv);

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(ima, c8(), nlabels);

  mln_VAR(res,
	  transform::distance_and_influence_zone_geodesic(lbl,
							  c8(),
							  mln_max(unsigned)));

  io::pgm::save(labeling::wrap(res.first()), argv[2]);
  io::ppm::save(labeling::colorize(value::rgb8(), res.second(), nlabels),
		argv[3]);

}
