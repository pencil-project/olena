// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

# include "xml_transform.hh"
# include "image_crop.hh"
# include "loader.hh"
# include "common.hh"
# include <iostream>

XmlTransform::XmlTransform(const QString& xml_file, const QString& image_file, const QString& output, const QString& file) :
  xml_file_(xml_file),
  image_(image_file),
  loader_(new Loader()),
  file_(file)
{
  QFile fi(image_);
  QFile fx(xml_file_);

  if (file_ != QString::Null())
    {
      output_dir_ = QDir::tempPath() + "/xml_transform/";
      tmp_ = true;
    }
  else
    {
      tmp_ = false;
      output_dir_ = output;
    }

  if (output_dir_ != QString::Null() && !loader_->set_output(output_dir_))
    {
      std::cout << "Cant't create " << output_dir_.toStdString() << "dir! Aborting..." << std::endl;
      abort();
    }
  else if (!fi.exists() && image_ != QString::Null())
    {
      std::cout << "Image file does not exist ! Aborting..." << std::endl;
      abort();
    }
  else if (!fx.exists())
    {
      std::cout << "XML file does not exist ! Aborting..." << std::endl;
      abort();
    }

  crop_ = new ImageCrop(xml_file_, image_, output_dir_);
}

XmlTransform::~XmlTransform()
{
}

void XmlTransform::fromBase64()
{
  crop_->from_base64();
}

void XmlTransform::toBase64(bool nocrop)
{
  if (!nocrop)
    crop_->crop_regions(tmp_);

  crop_->to_base64(file_, nocrop);
}

void XmlTransform::createPDF (bool crop, bool base64)
{
  if (loader_->xml_output(xml_file_, false, output_dir_))
    {
      if (!base64)
	{
	  if (crop)
	    crop_->crop_regions(tmp_);
	  else
	    crop_->save_image(output_dir_ + "image.png");
	}

      loader_->add_pdf_templates(crop, base64, output_dir_);
    }
  else
    abort();
}

void XmlTransform::createOpen ()
{
  if (loader_->xml_output(xml_file_, false, output_dir_))
    crop_->crop_regions(tmp_);
  else
    abort();
}

void XmlTransform::createSVG ()
{
  if (loader_->xml_output(xml_file_, false, output_dir_))
    loader_->add_svg_templates(output_dir_);
  else
    abort();
}

void XmlTransform::createHTML(bool base64)
{
  if (loader_->xml_output(xml_file_, true, output_dir_))
    {
      if (!base64)
	crop_->crop_regions(tmp_);
      //      loader_->add_html_templates(base64, output_dir_);
    }
  else
    abort();
}

