#ifndef SCIRESOURCENAN_H
#define SCIRESOURCENAN_H

#include "../SciMath/SciPixel.h"
#include "../SciMath/SciVector3.h"
#include "../SciMath/SciMatrix2x2.h"

// Typedefs
typedef unsigned int SciUint;
typedef const char* cstr;
typedef unsigned char byte;

template <class T>
class SciResourceNan
{
public:
	// Constructors
	SciResourceNan() : width_(0), height_(0), size_(0), data(nullptr) {}
	SciResourceNan(int width, int height = 1);
	SciResourceNan(cstr filename, int width, int height);

	// Rule of 5
	~SciResourceNan();
	SciResourceNan(const SciResourceNan<T>& other);
	SciResourceNan(SciResourceNan<T>&& other);
	SciResourceNan<T>& operator=(const SciResourceNan<T>& other);
	SciResourceNan<T>& operator=(SciResourceNan<T>&& other);

	// Accsess functions
	int Width() const { return width_; }
	int Height() const { return height_; }
	int Size() const { return size_; }
	SciVector2 Center() const; // Bot-Left corner of pixel is [0;0]
	const T* GetData() const { return data; }
	const T& operator[](int i) const;
	T& operator[](int i);
	const T& operator()(int x, int y) const; // Bottom-left corner origin
	T& operator()(int x, int y);

	// Resource Loading functions
	bool Init(int width, int height = 1); // Initialize memory for empty Nan
	// Supported file types: .tga (bitmaps), .obj (geometry)
	bool LoadFile(cstr filename, int width, int height = 1);

	// Basic operations
	SciResourceNan<T>& Fill(const T& filler);
	SciResourceNan<T>& SetZero(); // Set whole array to zero
	bool ReSize(int width, int height = 1); // Resize filled Nan, data is destroyed!

private:
	// Private functions
	void CleanUp();
	void AllocArray(int width, int height);
	// Spesific file loading functions
	/* TARGA file must be 32 bpp, correct resolution, with 0|0 left-bottom origin,
	no color map, no Image Identification */
	bool LoadTGA(cstr fname);

	// Data
	int width_;
	int height_;
	int size_;
	T* data;
};

// SciResourceNan typedefs
typedef SciResourceNan<SciPixel> SciSprite;
typedef SciResourceNan<SciPixel> SciTexture;
typedef SciResourceNan<SciPixel> SciImage;

// Usefull operations
// Invert alpha channel of sprite/texture/image
void invSpriteAlpha(const SciImage& image, SciResourceNan<float>& a_map);
// Apply color filter to sprite/texture/image
void applyFilter(SciImage& image, const SciPixel& color);
void rotateImage(const SciImage& source, SciImage& out, const SciMatrix2x2& rm);
// Ct - center translation from default center of image
void rotateImage(const SciImage& source, SciImage& out, const SciMatrix2x2& rm, const SciVector2& ct);
// Translate pixels from center by given vector
void moveImage(const SciImage& source, SciImage& out, const SciVector2 trns);
// Create line from vector
SciSprite drawLine(const SciVector2& vec, const SciPixel& color = blue_color);
//void saveImage(const SciImage& img, cstr fname);

#endif /* SCIRESOURCENAN_H */

// P.S. "Nan" for "Nanny" - ResourceNanny