// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include <mln/essential/2d.hh>
#include <mln/pw/all.hh>
#include <mln/level/compute.hh>
#include <mln/accu/maj_h.hh>

#include <scribo/table/rebuild.hh>
#include <scribo/table/erase.hh>
#include <scribo/table/extract_lines_with_rank.hh>
#include <scribo/make/debug_filename.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = argv[0];

  image2d<bool> input;
  io::pbm::load(input, argv[1]);
  logical::not_inplace(input);

  typedef util::couple<util::array<box2d>,util::array<box2d> > tblboxes_t;

  win::vline2d vline(51);
  win::hline2d hline(51);
  value::label_16 nbboxes;
  tblboxes_t lineboxes
	= table::extract_lines_with_rank(input, c8(), nbboxes,
					 vline, hline, 6, 6);

  value::label_8 ncells;
  image2d<value::label_8> tables
    = scribo::table::rebuild(input, lineboxes, 5, ncells).first();


  std::cout << "ncells (including background) = " << ncells << std::endl;
  image2d<value::rgb8> table_color = mln::labeling::colorize(value::rgb8(), tables, ncells);

  value::label_8 bg = level::compute(accu::maj_h<value::label_8>(), tables);

  image2d<value::rgb8> sup = level::convert(value::rgb8(), input);
  data::paste((table_color | pw::value(tables) != pw::cst(bg))
	  | (pw::value(sup) == pw::cst(literal::black)), sup);
  io::ppm::save(sup, scribo::make::debug_filename("table_color_sup.ppm"));

  io::ppm::save(table_color,
		scribo::make::debug_filename("table_cells.ppm"));
  io::pgm::save(tables, scribo::make::debug_filename("table_cells.pgm"));

  image2d<value::rgb8> input_rgb = level::convert(value::rgb8(), input);
  data::fill((input_rgb | pw::value(tables) == pw::cst(0u)).rw(), literal::red);
  io::ppm::save(input_rgb, scribo::make::debug_filename("table_superposed.ppm"));

  image2d<bool> in_wo_tables = table::erase(input, lineboxes);
  io::pbm::save(in_wo_tables,
      scribo::make::debug_filename("input_wo_tables.pbm"));

}
