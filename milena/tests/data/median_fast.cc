// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/data/fast_median.hh>

#include <mln/core/dpoints_pixter.hh>
#include <mln/core/pixel.hh>

#include "tests/data.hh"


namespace mln
{

  template <typename I, typename W>
  void test(I& input, const W& win)
  {
    mln_site(I) p;
    p.row() = p.col() = 1;

    {
      mln_qixter(I, W) qix(input, win, p);
      for_all(qix)
	std::cout << qix.val() << ' ';
      std::cout << " :  " << qix.center_val() << std::endl;
    }

    {
      pixel<I> pix(input, p);
      mln_qixter(I, W) qix(input, win, pix);
      for_all(qix)
	std::cout << qix.val() << ' ';
      std::cout << " :  " << qix.center_val() << std::endl;
    }
  }

}



int main()
{
  using namespace mln;
  using value::int_u8;

  // FIXME: Dead code.
//   {
//     win::rectangle2d rect(3, 3);
//     border::thickness = 4;
//     image2d<int_u8> ima(3, 3);
//     debug::iota(ima);
//     debug::println(ima);
//     test(ima, rect);
//   }


  {
    win::rectangle2d rect(51, 51);
    border::thickness = 52;
    
    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8>  out(lena.domain());
    
    data::fast_median(lena, rect, out);
    io::pgm::save(out, "median_fast-out.pgm");
  }

}
