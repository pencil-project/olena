# include <mln/core/image/image2d.hh>
# include <mln/core/site_set/p_line2d.hh>
# include <mln/core/image/safe.hh>
# include <mln/debug/println.hh>
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/pw/all.hh>



int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(3, 3);
  data::fill(ima, 0);
  
  safe_image<I> ima_(ima);
  data::paste(pw::cst(8) | p_line2d(point2d(-1,-1),
				     point2d( 3, 3)),
	       ima_);
  debug::println(ima);
}