// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/draw/label.cc
 *
 * \brief Tests on mln::draw::label.
 */


#include <mln/core/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>
#include <mln/draw/label.hh>
#include <mln/literal/all.hh>

#include <mln/core/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/core/neighb2d.hh>
#include <mln/labeling/blobs.hh>
#include <mln/display/save_and_show.hh>

#include "tests/data.hh"


int
main()
{
  using namespace mln;
  image2d<bool> input = io::pbm::load(MLN_IMG_DIR "/picasso.pbm");
  unsigned n;
  image2d<unsigned> ima = labeling::blobs(input, c4(), n);
  image2d<value::rgb8> out = draw::label (ima, literal::black);
  display::save_and_show(out, "display", 1);
}
