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

#ifndef MLN_UPSAMPLING_HQ3X_HH
# define MLN_UPSAMPLING_HQ3X_HH

/// \file
///
/// Produce an up-scaled image using hq3x algorithm.

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace upsampling
  {

    /// Subsampling FIXME : doxy
    template <typename I>
    inline
    mln_concrete(I)
    hq3x(const Image<I>& input);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      inline
      int
      rgb8toInt(value::rgb8 val)
      {
	int result = 0;

	result += val.blue();
	result = result << 8;
	result += val.green();
	result = result << 8;
	result += val.red();
	//result = result << 8;

	return result;
      }

      inline
      unsigned short
      rgb8toShort(value::rgb8 val)
      {
	unsigned short result = 0;

	result += val.red() >> 3;
	result = result << 6;
	result += val.green() >> 2;
	result = result << 5;
	result += val.blue() >> 3;

	return result;
      }

      inline
      unsigned int
      rgb8toYuv(value::rgb8 val)
      {
	unsigned int result = 0;

	result += (val.red() + val.green() + val.blue()) / 3;
	result = result << 8;
	result = result << 8;
	//result = result << 8;

	return result;
      }

      inline
      void
      intToRgb8(int in, value::rgb8& out)
      {
	out.blue() = in % 256;
	out.green() = (in >> 8) % 256;
	out.red() = (in >> 16) % 256;
      }

      template<typename T>
      inline
      void
      Interp1(image2d<T>& output, int row, int col, int c1, int c2)
      {
	intToRgb8((c1 * 3 + c2) >> 2, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp2(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 2 + c2 + c3) >> 2, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp3(image2d<T>& output, int row, int col, int c1, int c2)
      {
	intToRgb8((c1 * 7 + c2) / 8, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp4(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 2 + (c2 + c3) * 7) / 16, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp5(image2d<T>& output, int row, int col, int c1, int c2)
      {
	intToRgb8((c1 + c2) >> 1, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp6(image2d<T>& output, int row, int col, int c1)
      {
	intToRgb8(c1, output.at_(row, col));
      }


# define PIXEL00_1M  Interp1(output, i * 3, j * 3, c[5], c[1]);
# define PIXEL00_1U  Interp1(output, i * 3, j * 3, c[5], c[2]);
# define PIXEL00_1L  Interp1(output, i * 3, j * 3, c[5], c[4]);
# define PIXEL00_2   Interp2(output, i * 3, j * 3, c[5], c[4], c[2]);
# define PIXEL00_4   Interp4(output, i * 3, j * 3, c[5], c[4], c[2]);
# define PIXEL00_5   Interp5(output, i * 3, j * 3, c[4], c[2]);
# define PIXEL00_C   Interp6(output, i * 3, j * 3, c[5]);

# define PIXEL01_1   Interp1(output, i * 3, j * 3 + 1, c[5], c[2]);
# define PIXEL01_3   Interp3(output, i * 3, j * 3 + 1, c[5], c[2]);
# define PIXEL01_6   Interp1(output, i * 3, j * 3 + 1, c[2], c[5]);
# define PIXEL01_C   Interp6(output, i * 3, j * 3 + 1, c[5]);

# define PIXEL02_1M  Interp1(output, i * 3, j * 3 + 2, c[5], c[3]);
# define PIXEL02_1U  Interp1(output, i * 3, j * 3 + 2, c[5], c[2]);
# define PIXEL02_1R  Interp1(output, i * 3, j * 3 + 2, c[5], c[6]);
# define PIXEL02_2   Interp2(output, i * 3, j * 3 + 2, c[5], c[2], c[6]);
# define PIXEL02_4   Interp4(output, i * 3, j * 3 + 2, c[5], c[2], c[6]);
# define PIXEL02_5   Interp5(output, i * 3, j * 3 + 2, c[2], c[6]);
# define PIXEL02_C   Interp6(output, i * 3, j * 3 + 2, c[5]);

# define PIXEL10_1   Interp1(output, i * 3 + 1, j * 3, c[5], c[4]);
# define PIXEL10_3   Interp3(output, i * 3 + 1, j * 3, c[5], c[4]);
# define PIXEL10_6   Interp1(output, i * 3 + 1, j * 3, c[4], c[5]);
# define PIXEL10_C   Interp6(output, i * 3 + 1, j * 3, c[5]);

# define PIXEL11     Interp6(output, i * 3 + 1, j * 3 + 1, c[5]);

# define PIXEL12_1   Interp1(output, i * 3 + 1, j * 3 + 2, c[5], c[6]);
# define PIXEL12_3   Interp3(output, i * 3 + 1, j * 3 + 2, c[5], c[6]);
# define PIXEL12_6   Interp1(output, i * 3 + 1, j * 3 + 2, c[6], c[5]);
# define PIXEL12_C   Interp6(output, i * 3 + 1, j * 3 + 2, c[5]);

# define PIXEL20_1M  Interp1(output, i * 3 + 2, j * 3, c[5], c[7]);
# define PIXEL20_1D  Interp1(output, i * 3 + 2, j * 3, c[5], c[8]);
# define PIXEL20_1L  Interp1(output, i * 3 + 2, j * 3, c[5], c[4]);
# define PIXEL20_2   Interp2(output, i * 3 + 2, j * 3, c[5], c[8], c[4]);
# define PIXEL20_4   Interp4(output, i * 3 + 2, j * 3, c[5], c[8], c[4]);
# define PIXEL20_5   Interp5(output, i * 3 + 2, j * 3, c[8], c[4]);
# define PIXEL20_C   Interp6(output, i * 3 + 2, j * 3, c[5]);

# define PIXEL21_1   Interp1(output, i * 3 + 2, j * 3 + 1, c[5], c[8]);
# define PIXEL21_3   Interp3(output, i * 3 + 2, j * 3 + 1, c[5], c[8]);
# define PIXEL21_6   Interp1(output, i * 3 + 2, j * 3 + 1, c[8], c[5]);
# define PIXEL21_C   Interp6(output, i * 3 + 2, j * 3 + 1, c[5]);

# define PIXEL22_1M  Interp1(output, i * 3 + 2, j * 3 + 2, c[5], c[9]);
# define PIXEL22_1D  Interp1(output, i * 3 + 2, j * 3 + 2, c[5], c[8]);
# define PIXEL22_1R  Interp1(output, i * 3 + 2, j * 3 + 2, c[5], c[6]);
# define PIXEL22_2   Interp2(output, i * 3 + 2, j * 3 + 2, c[5], c[6], c[8]);
# define PIXEL22_4   Interp4(output, i * 3 + 2, j * 3 + 2, c[5], c[6], c[8]);
# define PIXEL22_5   Interp5(output, i * 3 + 2, j * 3 + 2, c[6], c[8]);
# define PIXEL22_C   Interp6(output, i * 3 + 2, j * 3 + 2, c[5]);

      inline
      bool
      Diff(int& YUV1, int& YUV2, int* RGBtoYUV,
	   const int trY, const int trU, const int trV,
	   const int Ymask, const int Umask, const int Vmask,
	   unsigned int w1, unsigned int w2)
      {
	YUV1 = RGBtoYUV[w1];
	YUV2 = RGBtoYUV[w2];
	return ((abs((YUV1 & Ymask) - (YUV2 & Ymask)) > trY) ||
		(abs((YUV1 & Umask) - (YUV2 & Umask)) > trU) ||
		(abs((YUV1 & Vmask) - (YUV2 & Vmask)) > trV));
      }

      inline
      void
      initLUTs(int LUT16to32[65536], int RGBtoYUV[65536])
      {
	int i, j, k, r, g, b, Y, u, v;

	for (i=0; i<65536; i++)
	  LUT16to32[i] = ((i & 0xF800) << 8) + ((i & 0x07E0) << 5) + ((i & 0x001F) << 3);

	for (i=0; i<32; i++)
	  for (j=0; j<64; j++)
	    for (k=0; k<32; k++)
	    {
	      r = i << 3;
	      g = j << 2;
	      b = k << 3;
	      Y = (r + g + b) >> 2;
	      u = 128 + ((r - b) >> 2);
	      v = 128 + ((-r + 2*g -b)>>3);
	      RGBtoYUV[ (i << 11) + (j << 5) + k ] = (Y<<16) + (u<<8) + v;
	    }
      }


      //FIXME: In Milena, nrows != row max
      template <typename T>
      inline
      mln_concrete(image2d<T>)
      hq3x_(const image2d<T>& input)
      {
	trace::entering("upsampling::impl::hq3x_");

	int nrows = geom::nrows(input);
	int ncols = geom::ncols(input);
        mln_concrete(image2d<T>) output(nrows * 3, ncols * 3);

	int   LUT16to32[65536];
	int   RGBtoYUV[65536];
	int   YUV1, YUV2;
	const  int   Ymask = 0x00FF0000;
	const  int   Umask = 0x0000FF00;
	const  int   Vmask = 0x000000FF;
	const  int   trY   = 0x00300000;
	const  int   trU   = 0x00000700;
	const  int   trV   = 0x00000006;

	initLUTs(LUT16to32, RGBtoYUV);

	unsigned int  w[10];
	unsigned int  c[10];

	//   +----+----+----+
	//   |    |    |    |
	//   | w1 | w2 | w3 |
	//   +----+----+----+
	//   |    |    |    |
	//   | w4 | w5 | w6 |
	//   +----+----+----+
	//   |    |    |    |
	//   | w7 | w8 | w9 |
	//   +----+----+----+
	//
	// w5 is the current point.

	for (int j = 0; j < ncols; j++)
	{
	  for (int i = 0; i < nrows; i++)
	  {
	    w[1] = rgb8toShort(input.at_(i - 1, j - 1));
	    w[2] = rgb8toShort(input.at_(i - 1, j));
	    w[3] = rgb8toShort(input.at_(i - 1, j + 1));
	    w[4] = rgb8toShort(input.at_(i, j - 1));
	    w[5] = rgb8toShort(input.at_(i, j));
	    w[6] = rgb8toShort(input.at_(i, j + 1));
	    w[7] = rgb8toShort(input.at_(i + 1, j - 1));
	    w[8] = rgb8toShort(input.at_(i + 1, j));
	    w[9] = rgb8toShort(input.at_(i + 1, j + 1));

	    int pattern = 0;
	    int flag = 1;

	    YUV1 = RGBtoYUV[w[5]];

	    for (int k = 1; k <= 9; k++)
	    {
	      if (k == 5)
		continue;

	      if (w[k] != w[5])
	      {
		YUV2 = RGBtoYUV[w[k]];
		if ((abs((YUV1 & Ymask) - (YUV2 & Ymask)) > trY) ||
		    (abs((YUV1 & Umask) - (YUV2 & Umask)) > trU) ||
		    (abs((YUV1 & Vmask) - (YUV2 & Vmask)) > trV))
		  pattern |= flag;
	      }
	      flag <<= 1;
	    }

	    for (int k = 1; k <= 9; k++)
	      c[k] = LUT16to32[w[k]];

	    switch (pattern)
	    {
	      case 0:
	      case 1:
	      case 4:
	      case 32:
	      case 128:
	      case 5:
	      case 132:
	      case 160:
	      case 33:
	      case 129:
	      case 36:
	      case 133:
	      case 164:
	      case 161:
	      case 37:
	      case 165:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 2:
	      case 34:
	      case 130:
	      case 162:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 16:
	      case 17:
	      case 48:
	      case 49:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 64:
	      case 65:
	      case 68:
	      case 69:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 8:
	      case 12:
	      case 136:
	      case 140:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 3:
	      case 35:
	      case 131:
	      case 163:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 6:
	      case 38:
	      case 134:
	      case 166:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 20:
	      case 21:
	      case 52:
	      case 53:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 144:
	      case 145:
	      case 176:
	      case 177:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 192:
	      case 193:
	      case 196:
	      case 197:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 96:
	      case 97:
	      case 100:
	      case 101:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 40:
	      case 44:
	      case 168:
	      case 172:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 9:
	      case 13:
	      case 137:
	      case 141:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 18:
	      case 50:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_1M
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 80:
	      case 81:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_1M
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 72:
	      case 76:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_1M
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 10:
	      case 138:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 66:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 24:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 7:
	      case 39:
	      case 135:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 148:
	      case 149:
	      case 180:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 224:
	      case 228:
	      case 225:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 41:
	      case 169:
	      case 45:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 22:
	      case 54:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 208:
	      case 209:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 104:
	      case 108:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 11:
	      case 139:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 19:
	      case 51:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_1L
		      PIXEL01_C
		      PIXEL02_1M
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL01_6
		      PIXEL02_5
		      PIXEL12_1
		  }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 146:
	      case 178:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_1M
		      PIXEL12_C
		      PIXEL22_1D
		  }
		  else
		  {
		    PIXEL01_1
		      PIXEL02_5
		      PIXEL12_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    break;
		}
	      case 84:
	      case 85:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL02_1U
		      PIXEL12_C
		      PIXEL21_C
		      PIXEL22_1M
		  }
		  else
		  {
		    PIXEL02_2
		      PIXEL12_6
		      PIXEL21_1
		      PIXEL22_5
		  }
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    break;
		}
	      case 112:
	      case 113:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL20_1L
		      PIXEL21_C
		      PIXEL22_1M
		  }
		  else
		  {
		    PIXEL12_1
		      PIXEL20_2
		      PIXEL21_6
		      PIXEL22_5
		  }
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    break;
		}
	      case 200:
	      case 204:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_C
		      PIXEL20_1M
		      PIXEL21_C
		      PIXEL22_1R
		  }
		  else
		  {
		    PIXEL10_1
		      PIXEL20_5
		      PIXEL21_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    break;
		}
	      case 73:
	      case 77:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_1U
		      PIXEL10_C
		      PIXEL20_1M
		      PIXEL21_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL10_6
		      PIXEL20_5
		      PIXEL21_1
		  }
		  PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    PIXEL22_1M
		    break;
		}
	      case 42:
	      case 170:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		      PIXEL01_C
		      PIXEL10_C
		      PIXEL20_1D
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_1
		      PIXEL10_6
		      PIXEL20_2
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 14:
	      case 142:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		      PIXEL01_C
		      PIXEL02_1R
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_6
		      PIXEL02_2
		      PIXEL10_1
		  }
		  PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 67:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 70:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 28:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 152:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 194:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 98:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 56:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 25:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 26:
	      case 31:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL10_3
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL11
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 82:
	      case 214:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 88:
	      case 248:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL22_4
		    }
		  break;
		}
	      case 74:
	      case 107:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		  }
		  PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 27:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 86:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 216:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 106:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 30:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 210:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 120:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 75:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 29:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 198:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 184:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 99:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 57:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 71:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 156:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 226:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 60:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 195:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 102:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 153:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 58:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 83:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 92:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 202:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 78:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 154:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 114:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 89:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 90:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 55:
	      case 23:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_1L
		      PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL01_6
		      PIXEL02_5
		      PIXEL12_1
		  }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 182:
	      case 150:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		      PIXEL22_1D
		  }
		  else
		  {
		    PIXEL01_1
		      PIXEL02_5
		      PIXEL12_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_2
		    PIXEL21_1
		    break;
		}
	      case 213:
	      case 212:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL02_1U
		      PIXEL12_C
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL02_2
		      PIXEL12_6
		      PIXEL21_1
		      PIXEL22_5
		  }
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    break;
		}
	      case 241:
	      case 240:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL20_1L
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_1
		      PIXEL20_2
		      PIXEL21_6
		      PIXEL22_5
		  }
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    break;
		}
	      case 236:
	      case 232:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_C
		      PIXEL20_C
		      PIXEL21_C
		      PIXEL22_1R
		  }
		  else
		  {
		    PIXEL10_1
		      PIXEL20_5
		      PIXEL21_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    break;
		}
	      case 109:
	      case 105:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_1U
		      PIXEL10_C
		      PIXEL20_C
		      PIXEL21_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL10_6
		      PIXEL20_5
		      PIXEL21_1
		  }
		  PIXEL01_1
		    PIXEL02_2
		    PIXEL11
		    PIXEL12_1
		    PIXEL22_1M
		    break;
		}
	      case 171:
	      case 43:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		      PIXEL20_1D
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_1
		      PIXEL10_6
		      PIXEL20_2
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 143:
	      case 15:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL02_1R
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_6
		      PIXEL02_2
		      PIXEL10_1
		  }
		  PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 124:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 203:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 62:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 211:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 118:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 217:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 110:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 155:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 188:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 185:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 61:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 157:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 103:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 227:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 230:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 199:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 220:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_3
		      PIXEL21_3
		      PIXEL22_4
		  }
		  break;
		}
	      case 158:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL01_3
		      PIXEL02_4
		      PIXEL12_3
		  }
		  PIXEL10_C
		    PIXEL11
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 234:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1M
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1R
		    break;
		}
	      case 242:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_1L
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 59:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL02_1M
		  }
		  else
		  {
		    PIXEL02_2
		  }
		  PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 121:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL22_1M
		  }
		  else
		  {
		    PIXEL22_2
		  }
		  break;
		}
	      case 87:
		{
		  PIXEL00_1L
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_1M
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 79:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1R
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 122:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL22_1M
		  }
		  else
		  {
		    PIXEL22_2
		  }
		  break;
		}
	      case 94:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL01_3
		      PIXEL02_4
		      PIXEL12_3
		  }
		  PIXEL10_C
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 218:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_3
		      PIXEL21_3
		      PIXEL22_4
		  }
		  break;
		}
	      case 91:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL02_1M
		  }
		  else
		  {
		    PIXEL02_2
		  }
		  PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 229:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 167:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 173:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 181:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 186:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 115:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 93:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 206:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 205:
	      case 201:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_1M
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 174:
	      case 46:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_1M
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 179:
	      case 147:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_1M
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 117:
	      case 116:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_1M
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 189:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 231:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 126:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 219:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		      PIXEL10_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 125:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_1U
		      PIXEL10_C
		      PIXEL20_C
		      PIXEL21_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL10_6
		      PIXEL20_5
		      PIXEL21_1
		  }
		  PIXEL01_1
		    PIXEL02_1U
		    PIXEL11
		    PIXEL12_C
		    PIXEL22_1M
		    break;
		}
	      case 221:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL02_1U
		      PIXEL12_C
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL02_2
		      PIXEL12_6
		      PIXEL21_1
		      PIXEL22_5
		  }
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL10_C
		    PIXEL11
		    PIXEL20_1M
		    break;
		}
	      case 207:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL02_1R
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_6
		      PIXEL02_2
		      PIXEL10_1
		  }
		  PIXEL11
		    PIXEL12_1
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 238:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_C
		      PIXEL20_C
		      PIXEL21_C
		      PIXEL22_1R
		  }
		  else
		  {
		    PIXEL10_1
		      PIXEL20_5
		      PIXEL21_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1R
		    PIXEL11
		    PIXEL12_1
		    break;
		}
	      case 190:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		      PIXEL22_1D
		  }
		  else
		  {
		    PIXEL01_1
		      PIXEL02_5
		      PIXEL12_6
		      PIXEL22_2
		  }
		  PIXEL00_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL20_1D
		    PIXEL21_1
		    break;
		}
	      case 187:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		      PIXEL20_1D
		  }
		  else
		  {
		    PIXEL00_5
		      PIXEL01_1
		      PIXEL10_6
		      PIXEL20_2
		  }
		  PIXEL02_1M
		    PIXEL11
		    PIXEL12_C
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 243:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL20_1L
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_1
		      PIXEL20_2
		      PIXEL21_6
		      PIXEL22_5
		  }
		  PIXEL00_1L
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL10_1
		    PIXEL11
		    break;
		}
	      case 119:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_1L
		      PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL01_6
		      PIXEL02_5
		      PIXEL12_1
		  }
		  PIXEL10_1
		    PIXEL11
		    PIXEL20_1L
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 237:
	      case 233:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_2
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 175:
	      case 47:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_2
		    break;
		}
	      case 183:
	      case 151:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_2
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 245:
	      case 244:
		{
		  PIXEL00_2
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 250:
		{
		  PIXEL00_1M
		    PIXEL01_C
		    PIXEL02_1M
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL22_4
		    }
		  break;
		}
	      case 123:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		  }
		  PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 95:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL10_3
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL11
		    PIXEL20_1M
		    PIXEL21_C
		    PIXEL22_1M
		    break;
		}
	      case 222:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 252:
		{
		  PIXEL00_1M
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 249:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL12_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL12_3
			PIXEL22_4
		    }
		  break;
		}
	      case 235:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		  }
		  PIXEL02_1M
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 111:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 63:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
			PIXEL12_C
		    }
		    else
		    {
		      PIXEL02_4
			PIXEL12_3
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1M
		    break;
		}
	      case 159:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL10_3
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 215:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 246:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 254:
		{
		  PIXEL00_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_C
			PIXEL02_C
		    }
		    else
		    {
		      PIXEL01_3
			PIXEL02_4
		    }
		  PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_4
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL21_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_3
		      PIXEL21_3
		      PIXEL22_2
		  }
		  break;
		}
	      case 253:
		{
		  PIXEL00_1U
		    PIXEL01_1
		    PIXEL02_1U
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 251:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL01_3
		  }
		  PIXEL02_1M
		    PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_C
			PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL10_3
			PIXEL20_2
			PIXEL21_3
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL12_C
		      PIXEL22_C
		  }
		  else
		  {
		    PIXEL12_3
		      PIXEL22_4
		  }
		  break;
		}
	      case 239:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    PIXEL02_1R
		    PIXEL10_C
		    PIXEL11
		    PIXEL12_1
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    PIXEL22_1R
		    break;
		}
	      case 127:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL01_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_2
		      PIXEL01_3
		      PIXEL10_3
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL02_4
		      PIXEL12_3
		  }
		  PIXEL11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
			PIXEL21_C
		    }
		    else
		    {
		      PIXEL20_4
			PIXEL21_3
		    }
		  PIXEL22_1M
		    break;
		}
	      case 191:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1D
		    PIXEL21_1
		    PIXEL22_1D
		    break;
		}
	      case 223:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		      PIXEL10_C
		  }
		  else
		  {
		    PIXEL00_4
		      PIXEL10_3
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_C
		      PIXEL02_C
		      PIXEL12_C
		  }
		  else
		  {
		    PIXEL01_3
		      PIXEL02_2
		      PIXEL12_3
		  }
		  PIXEL11
		    PIXEL20_1M
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL21_C
			PIXEL22_C
		    }
		    else
		    {
		      PIXEL21_3
			PIXEL22_4
		    }
		  break;
		}
	      case 247:
		{
		  PIXEL00_1L
		    PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_1
		    PIXEL11
		    PIXEL12_C
		    PIXEL20_1L
		    PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	      case 255:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_C
		  }
		  else
		  {
		    PIXEL00_2
		  }
		  PIXEL01_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL02_C
		    }
		    else
		    {
		      PIXEL02_2
		    }
		  PIXEL10_C
		    PIXEL11
		    PIXEL12_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL20_C
		    }
		    else
		    {
		      PIXEL20_2
		    }
		  PIXEL21_C
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL22_C
		    }
		    else
		    {
		      PIXEL22_2
		    }
		  break;
		}
	    }
	  }
	}

	trace::exiting("upsampling::impl::hq3x_");
	return output;
      }

    } // end of namespace mln::upsampling::impl


    template <typename I>
    inline
    mln_concrete(I)
    hq3x(const Image<I>& input)
    {
      trace::entering("upsampling::hq3x");
      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output;

      output = impl::hq3x_(exact(input));

      trace::exiting("upsampling::hq3x");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::upsampling

} // end of namespace mln


#endif // ! MLN_UPSAMPLING_HQ3X_HH