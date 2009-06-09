// Copyright (C) 2009
 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/essential/2d.hh>
#include <mln/transform/distance_geodesic.hh>
#include <mln/core/var.hh>
#include <mln/fun/l2l/wrap.hh>
#include <mln/win/hline2d.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/watershed/superpose.hh>

#include <scribo/make/debug_filename.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::label_16;
  using value::label_8;
  using value::rgb8;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  scribo::make::internal::debug_filename_prefix = "morpho";

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(input, c8(), nlabels);

  mln_VAR(dmap, transform::distance_geodesic(lbl, c8(), mln_max(unsigned)));

  io::pgm::save(level::transform(dmap, fun::l2l::wrap<label_8>()), "dmap.pgm");

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::disk2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_disk_51.pgm"));
    image2d<float> r = morpho::tree:compute_attribute_image(accu::site_set::rectangularity<point2d>(),clo);
    image2d<point2d> r = morpho::tree:max(r,c8());
  }
}
