#include <iostream>


#include <mln/accu/max.hh>
#include <mln/util/pix.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/concept/function.hh>


#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>


#include <mln/level/sort_psites.hh>
#include <mln/morpho/tree/data.hh>

#include <../../theo/color/change_attributes.hh>



#include "propagate_node.hh"
#include "propagate_if.hh"
#include "accumulator/arg_max.hh"
#include "run.hh"

void usage(char** argv)
{
  std::cerr << "usage: " << argv[0] << " input" << std::endl;
  abort();
}

void dsp(const char* str)
{
  std::cout << "*********************" << std::endl
	    << "** " << str << std::endl
	    << "*********************" << std::endl;
}

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
    usage(argv);


  typedef image2d<int_u8> I;
  I input, dup;


  io::pgm::load(input, argv[1]);


  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;
  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());

  dsp("Input:");
  display_tree_attributes(tree, input);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_ancestors(point2d(1, 4), tree, dup, 0);
  dsp("Propagate node to ancestors  : (point2d(1, 4), tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_ancestors(point2d(1, 4), tree, dup);
  dsp("Propagate node to ancestors  : (point2d(1, 4), tree, dup)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_descendants(point2d(0, 2), tree, dup, 0, 0);
  dsp("Propagate node to descendants  : (point2d(0, 2), tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_descendants(point2d(0, 2), tree, dup);
  dsp("Propagate node to descendants  : (point2d(0, 2), tree, dup)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_if_value(tree, dup, morpho::tree::asc_propagation (), 117, 0);
  dsp("Propagate value to ancestors  : (117, tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_if_value(tree, dup, morpho::tree::desc_propagation (), 117, 0);
  dsp("Propagate value to descendants  : (117, tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_if_value(tree, dup, morpho::tree::desc_propagation (), 117);
  dsp("Propagate value to descendants  : (117, tree, dup)");
  display_tree_attributes(tree, dup);
}



