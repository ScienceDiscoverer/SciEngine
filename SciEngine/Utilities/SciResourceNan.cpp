#include <cassert>
#include <string>
#include <fstream>
#include "SciResourceNan.h"
#include "../SciMath/SciMath.h"

////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#include "SciDebug.h"
//str outd;
/////////////////// DEBUG INCLUDES ///////////////////

// Supported file types
enum FileType { Bitmap_TGA, Mesh_OBJ, No_Sup };

// Helper structs
struct TgaHeader
{
	byte image_type;
	int width;
	int height;
	byte bpp;
};

struct SpriteBox
{
	SciVector2 orig; // Origin point
	SciVector2 x, y;
	float mx_sqr_recip, my_sqr_recip; // Reciprocals of magnitudes squared
};
// End of Helper structs

// Helper functions
// Cast string characters to lower case
void strToLower(std::string& str)
{
	for(auto& c : str)
	{
		c = tolower(c);
	}
}

// Convert signed byte to normilised float (0 <= f <= 1)
float charToFloat(char sb)
{
	return (float)((byte)sb) / 255.0f;
}

// Determine file type based on file name
FileType deduceType(cstr fname)
{
	std::string f_name(fname);
	std::size_t last_dot = f_name.rfind('.');

	if(last_dot == std::string::npos)
	{
		return No_Sup; // File name has no extension
	}

	std::string f_type = f_name.substr(last_dot+1);
	strToLower(f_type);

	// Test for supported file types
	if(!f_type.compare("tga"))
	{
		return Bitmap_TGA;
	}

	if(!f_type.compare("obj"))
	{
		return Mesh_OBJ;
	}
	
	return No_Sup; // File is not supported
}

// Pick color from image, based on UV coordinates box
const SciPixel& sampleImage(const SciVector2& pos, const SciImage& img, const SpriteBox& box)
{
	// Calculate position reletive to UV origin
	SciVector2 uv_pos = pos - box.orig;

	// Project position upon x and y vectors of the box
	const float p_dot_x = uv_pos.Dot(box.x);
	if(p_dot_x < 0.0f)
	{
		return zero_color; // Sample postion is outside of the box
	}
	const float p_dot_y = uv_pos.Dot(box.y);
	if(p_dot_y < 0.0f)
	{
		return zero_color; // Sample postion is outside of the box
	}

	// Find UV coordinates of sample position
	const float u = p_dot_x * box.mx_sqr_recip;
	if(u >= 1.0f)
	{
		return zero_color; // Sample postion is outside of the box
	}
	const float v = p_dot_y * box.my_sqr_recip;
	if(v >= 1.0f)
	{
		return zero_color; // Sample postion is outside of the box
	}

	return img(sciFloorI(u * (float)img.Width()), sciFloorI(v * (float)img.Height()));
}
// End of Helper functions

// Constructors
template <class T>
SciResourceNan<T>::SciResourceNan(int width, int height) : width_(width), height_(height)
{
	assert(width > 0 && height > 0);
	size_ = width * height;
	data = new T[size_];
}

template <class T>
SciResourceNan<T>::SciResourceNan(cstr filename, int width, int height)
{
	LoadFile(filename, width, height);
}
// End of Constructors

// Rule of 5
template <class T>
SciResourceNan<T>::~SciResourceNan()
{
	CleanUp();
}

// Deep copy
template <class T>
SciResourceNan<T>::SciResourceNan(const SciResourceNan<T>& other)
{
	AllocArray(other.width_, other.height_);
	memcpy(data, other.data, size_*sizeof(T));
}

template <class T>
SciResourceNan<T>::SciResourceNan(SciResourceNan<T>&& other) : data(other.data),
	size_(other.size_), width_(other.width_), height_(other.height_)
{
	other.data = nullptr;
	other.size_ = 0;
	other.width_ = 0;
	other.height_ = 0;
}

template <class T>
SciResourceNan<T>& SciResourceNan<T>::operator=(const SciResourceNan<T>& other)
{
	if(this != &other) // If attempted to assign object to itself, abort
	{
		CleanUp();
		AllocArray(other.width_, other.height_);
		memcpy(data, other.data, size_*sizeof(T));
	}

	return *this;
}

template <class T>
SciResourceNan<T>& SciResourceNan<T>::operator=(SciResourceNan<T>&& other)
{
	if(this != &other) // If attempted to move itself, abort
	{
		CleanUp();
		data = other.data;
		size_ = other.size_;
		width_ = other.width_;
		height_ = other.height_;
		other.data = nullptr;
		other.size_ = 0;
		other.width_ = 0;
		other.height_ = 0;
	}

	return *this;
}
// End of Rule of 5

// Accsess functions
// Bot-Left corner of pixel is [0;0]
template <class T>
SciVector2 SciResourceNan<T>::Center() const
{
	SciVector2 tmp;
	tmp.x = (float)width_ / 2;
	tmp.y = (float)height_ / 2;
	return tmp;
}

template <class T>
const T& SciResourceNan<T>::operator[](int i) const
{
	assert(i >= 0 && i < size_);
	return data[i];
}

template <class T>
T& SciResourceNan<T>::operator[](int i)
{
	assert(i >= 0 && i < size_);
	return data[i];
}

// Bottom-left corner origin (OpenGL style)
template <class T>
const T& SciResourceNan<T>::operator()(int x, int y) const
{
	assert(x >= 0 && x < width_ && y >= 0 && y < height_);
	return data[width_*y + x];
}

template <class T>
T& SciResourceNan<T>::operator()(int x, int y)
{
	assert(x >= 0 && x < width_ && y >= 0 && y < height_);
	return data[width_*y + x];
}
// End of Accsess functions

// Resource Loading functions
// Initialize memory for empty Nan
template <class T>
bool SciResourceNan<T>::Init(int width, int height)
{
	assert(width > 0 && height > 0);
	
	if(!size_)
	{
		AllocArray(width, height);
		return true;
	}
	else
	{
		return false;
	}
}

// Supported file types: .tga (bitmaps), .obj (geometry)
template <class T>
bool SciResourceNan<T>::LoadFile(cstr filename, int width, int height)
{
	assert(width > 0 && height > 0);

	FileType ftype = deduceType(filename);
	if(ftype == No_Sup)
	{
		return false; // Exit function, file type is not supported
	}

	CleanUp();
	AllocArray(width, height);
	bool success;

	switch(ftype)
	{
	case Bitmap_TGA:
		success = LoadTGA(filename);
		break;
	case Mesh_OBJ:
		// Call obj loading function...
		break;
	default:
		break;
	}

	return success; // File successfully loaded (or not)
}
// End of Resource Loading functions

// Basic operations
template <class T>
SciResourceNan<T>& SciResourceNan<T>::Fill(const T& filler)
{
	if(sizeof(T) > 8) // Memcpy will be more fast for big types like pixels
	{
		for(int i = 0; i < size_; ++i)
		{
			memcpy(data+i, &filler, sizeof(T));
		}
	}
	else
	{
		for(int i = 0; i < size_; ++i)
		{
			data[i] = filler;
		}
	}

	return *this;
}

// Set whole array to zero
template <class T>
SciResourceNan<T>& SciResourceNan<T>::SetZero()
{
	memset(data, 0, size_*sizeof(T));
	return *this;
}

// Resize already filled Nanny
template <class T>
bool SciResourceNan<T>::ReSize(int width, int height)
{
	assert(width > 0 && height > 0);

	if(size_)
	{
		CleanUp();
		AllocArray(width, height);
		return true;
	}
	else
	{
		return false;
	}
}
// End of Basic operations

// Private functions
template <class T>
void SciResourceNan<T>::CleanUp()
{
	if(data)
	{
		delete[] data;
	}
}

// Allocate heap memory for data, and initialize dimensions
template <class T>
void SciResourceNan<T>::AllocArray(int width, int height)
{
	size_ = width * height;
	width_ = width;
	height_ = height;
	data = new T[size_];
}

// Spesific file loading functions
// Dummy function for non-SciPixel types
template <class T>
bool SciResourceNan<T>::LoadTGA(cstr fname)
{
	return false;
}

/* TARGA file must be 32 bpp, correct resolution, with 0|0 left-bottom origin,
no Color Map, no Image Identification */
template <>
bool SciResourceNan<SciPixel>::LoadTGA(cstr fname)
{
	std::fstream img(fname, std::fstream::in | std::fstream::binary | std::fstream::ate);
	
	if(!img.is_open() || !img.tellg())
	{
		return false; // Error, file could not be opened or it's empty
	}
	
	char b;
	TgaHeader head;
	img.seekg(0); // Return to the beginning of a file

	while(img.get(b))
	{
		switch((int)img.tellg()-1)
		{
		case 2:
			head.image_type = b;
			break;

		case 12:
		{
			byte low_byte = b; // Save low order byte of width (little-endian)
			img.get(b); // Move 1 byte forward, to high order byte
			int high_byte = (byte)b;
			head.width = (high_byte << 8) + low_byte; // Combine 2 bytes to form width

			img.get(b); // Move on to height bytes
			low_byte = b;
			img.get(b);
			high_byte = (byte)b;
			head.height = (high_byte << 8) + low_byte; // Combine 2 bytes to form height

			img.get(b); // Move on to "bits per pixel" byte
			head.bpp = b;
			break;
		}

		case 18:
			if(head.width != width_ || head.height != height_ || head.bpp != 32)
			{
				return false; // Exit function, incorrect image dimentions or bpp
			}
			
			switch(head.image_type)
			{
			case 2: // Unmapped (raw) RGB image
				for(int i = 0; i < size_; ++i)
				{
					data[i].b = charToFloat(b);
					img.get(b); // Move to green
					data[i].g = charToFloat(b);
					img.get(b); // Move to red
					data[i].r = charToFloat(b);
					img.get(b); // Move to alpha
					data[i].a = charToFloat(b);
					img.get(b); // Move to next pixel
				}
				break;

			case 10: // Run Length Encoded (RLE) RGB image
				for(int i = 0; i < size_; ++i)
				{
					const byte mask_id = 128;
					const byte mask_length = 127;
					const byte packet = b; // Packet header
					const byte pl = (packet & mask_length) + 1; // Packet length
					const int pix_to_fill = i + pl; // bound on how much pixels to fill

					if(packet & mask_id) // Run-length packet
					{
						SciPixel run_color;
						img.get(b); // Move to color bytes
						run_color.b = charToFloat(b);
						img.get(b); // Move to green
						run_color.g = charToFloat(b);
						img.get(b); // Move to red
						run_color.r = charToFloat(b);
						img.get(b); // Move to alpha
						run_color.a = charToFloat(b);

						for(int j = i; j < pix_to_fill; ++j)
						{
							// Set run-color to all pixels of packet length
							data[j] = run_color;
							++i;
						}
						--i;

						img.get(b); // Move to next packet's header byte
					}
					else // Raw packet
					{
						for(int j = i; j < pix_to_fill; ++j)
						{
							img.get(b); // Move to pixel data (or next color)
							data[j].b = charToFloat(b);
							img.get(b); // Move to green
							data[j].g = charToFloat(b);
							img.get(b); // Move to red
							data[j].r = charToFloat(b);
							img.get(b); // Move to alpha
							data[j].a = charToFloat(b);

							++i;
						}
						--i;

						img.get(b); // Move to next packet's header byte
					}
				}
				break;

			default:
				return false; // Exit function, unsupported TARGA file type
				break;
			}
			break;

		default:
			break;
		}
	}

	return true; // Image successfully loaded
}
// End of Private functions

// Usefull operations
// Invert alpha channel of sprite/texture/image
void invSpriteAlpha(const SciImage& image, SciResourceNan<float>& a_map)
{
	assert(image.Size() == a_map.Size());

	for(int i = 0; i < image.Size(); ++i)
	{
		a_map[i] = image[i].GetInverseAlpha();
	}
}

// Apply color filter to sprite/texture/image
void applyFilter(SciImage& image, const SciPixel& color)
{
	for(int i = 0; i < image.Size(); ++i)
	{
		image[i] *= color;
	}
}

void rotateImage(const SciImage& source, SciImage& out, const SciMatrix2x2& rm)
{
	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	if(!debug_on)
	{
		debug = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_POPUP,
			100, 0, 400, 400, NULL, NULL, NULL, NULL);
		d_info = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_CHILD,
			0, 0, 390, 390, debug, NULL, NULL, NULL);
		debug_on = true;
	}

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////
	
	const float w = (float)source.Width();
	const float h = (float)source.Height();

	// Calculate center of the image - assume [x,y] to be left-bot corner of pixel
	const SciVector2 cent(w/2, h/2);

	// Calculate box corner points, relative to center of the image
	SciVector2 bl = -cent; // Bottom-left point
	SciVector2 br = SciVector2(w, 0.0f) - cent; // Bottom-right point
	SciVector2 tl = SciVector2(0.0f, h) - cent;
	// Rotate corner points around center
	bl = rm * bl;
	br = rm * br;
	tl = rm * tl;

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	outd += str("cent: ") + tstr(cent) + nl;
	outd += str("bl_after_rot: ") + tstr(bl) + nl;
	outd += str("RotMatRow0: ") + tstr(rm.GetRow(0)) + nl;
	outd += str("RotMatRow1: ") + tstr(rm.GetRow(1)) + nl;


	SetWindowTextA(d_info, outd.c_str());
	outd.clear();

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	// Fill up box
	SpriteBox box;
	box.orig = bl;
	box.x = br - bl;
	box.y = tl - bl;
	box.mx_sqr_recip = 1.0f / (w * w);
	box.my_sqr_recip = 1.0f / (h * h);

	// Calculate center of rotated image
	const SciVector2 new_cent((float)out.Width()/2, (float)out.Height()/2);

	// Sample and pick colors from rotated image
	for(int i = 0; i < out.Height(); ++i)
	{
		for(int j = 0; j < out.Width(); ++j)
		{
			// Calculate position of output image's current pixel's center
			SciVector2 pos((float)j + 0.5f, (float)i + 0.5f);
			pos -= new_cent;
			out(j, i) = sampleImage(pos, source, box);
		}
	}
}

// Ct - center translation from default center of image
void rotateImage(const SciImage& source, SciImage& out, const SciMatrix2x2& rm, const SciVector2& ct)
{
	const float w = (float)source.Width();
	const float h = (float)source.Height();
	
	// Calculate center of the image - assume [x,y] to be left-bot corner of pixel
	SciVector2 cent(w/2, h/2);
	// Translate to new center
	cent += ct;
	
	// Calculate box corner points, relative to center of the image
	SciVector2 bl = -cent; // Bottom-left point
	SciVector2 br = SciVector2(w, 0.0f) - cent; // Bottom-right point
	SciVector2 tl = SciVector2(0.0f, h) - cent;
	// Rotate corner points around center
	bl = rm * bl;
	br = rm * br;
	tl = rm * tl;

	// Fill up box
	SpriteBox box;
	box.orig = bl;
	box.x = br - bl;
	box.y = tl - bl;
	box.mx_sqr_recip = 1.0f / (w * w);
	box.my_sqr_recip = 1.0f / (h * h);

	// Calculate center of rotated image
	const SciVector2 new_cent((float)out.Width()/2, (float)out.Height()/2);

	// Sample and pick colors from rotated image
	for(int i = 0; i < out.Height(); ++i)
	{
		for(int j = 0; j < out.Width(); ++j)
		{
			// Calculate position of output image's current pixel's center
			SciVector2 pos((float)j + 0.5f, (float)i + 0.5f);
			pos -= new_cent;
			out(j, i) = sampleImage(pos, source, box);
		}
	}
}

// Translate pixels from center by given vector
void moveImage(const SciImage& source, SciImage& out, const SciVector2 trns)
{

}

// Create line from vector
SciSprite drawLine(const SciVector2& vec, const SciPixel& color)
{
	const float mag = vec.Mag();
	const int size = sciCeilingI(mag);
	SciSprite line(size*2+1, size*2+1);
	line.SetZero();

	for(int i = 0; i < size; ++i)
	{
		const float t = (float)i / size;
		SciVector2 pix = vec * t;
		pix += SciVector2(mag, mag);
		line(sciFloorI(pix.x), sciFloorI(pix.y)) = color;
	}

	return line;
}

/*void saveImage(const SciImage& img, cstr fname)
{
	///////////////////////// TEMPORIAL OTSIDE CODE ///////////////////////

	//if(img.Width() == 0 || img.Height() == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
	// ATTENTION!!! VISUAL STUDIO RELESE CONFIG HAS TROUBLES WITH OUTPUT STREAM!
	std::ofstream ofs;
	ofs.open(fname, std::ios::out | std::ios::binary); // need to spec. binary mode for Windows users
	//if(ofs.fail()) return;
	ofs << "P6\n" << img.Width() << " " << img.Height() << "\n255\n";
	unsigned char r, g, b;
	// loop over each pixel in the image, clamp and convert to byte format
	for(int i = 0; i < img.Width() * img.Height(); ++i) {
		r = unsigned char(sciMin(1.f, img[i].r) * 255);
		g = unsigned char(sciMin(1.f, img[i].g) * 255);
		b = unsigned char(sciMin(1.f, img[i].b) * 255);
		ofs << r << g << b;
	}
	ofs.close();

	///////////////// IMPLEMENT MY OWN CODE FOR TARGA LATER /////////////////
}*/
// End of Usefull operations

// Explicit template instantiations
template class SciResourceNan<SciPixel>;
template class SciResourceNan<SciVector3>;
template class SciResourceNan<SciUint>;
template class SciResourceNan<float>;