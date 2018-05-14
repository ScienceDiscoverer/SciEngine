#ifndef SCIARRAY_H
#define SCIARRAY_H

#include <cassert>
#include <cstring>

template <class T>
class SciArray
{
public:
	// Constructors
	SciArray() : size_(0), cur_size(0), data(nullptr), ds(20) {}
	SciArray(int size);

	// Rule of 5
	~SciArray();
	SciArray(const SciArray<T>& other) = delete;
	SciArray(SciArray<T>&& other) = delete;
	SciArray<T>& operator=(const SciArray<T>& other) = delete;
	SciArray<T>& operator=(SciArray<T>&& other) = delete;

	// Access operators
	T& operator[](int i) { return data[i]; }
	const T& operator[](int i) const { return data[i]; }
	T& PeekBack() const { return data[cur_size-1]; } // Access last element
	int Size() const { return cur_size; }

	// Data manipulation
	void Push(const T& obj); // Add object at the end of array
	void Pop(int index); // Remove object at index
	void Clean() { cur_size = 0; } // Remove all objects from array
	// Type must have overloaded comparasion operators to be sorted
	void Sort(bool low_to_high = true);
	// Type must have overloaded == operator to support romoval by value
	// first_inst - remove only 1st instance only (true), all copies (false)
	void Remove(const T& obj, bool first_inst = true);
	T* GetData() { return data; }

private:
	// Private functions
	void AllocArray();
	void AddObj(const T& obj) { data[cur_size++] = obj; }
	void ExpandArray();
	void CleanUp();

	// Data
	int ds; // Delta Size
	int cur_size; // Current size used
	int size_; // Real size
	T* data;
};

// Template Implementation
// Constructors
template <class T>
SciArray<T>::SciArray(int size) : size_(size), cur_size(0), ds(20)
{
	assert(size >= 0);

	if(size)
	{
		AllocArray();
	}
	else
	{
		data = nullptr;
	}
}
// End of Constructors

// Rule of 5
template <class T>
SciArray<T>::~SciArray()
{
	CleanUp();
}
// End of Rule of 5

// Data manipulation
// Add object at the end of array
template <class T>
void SciArray<T>::Push(const T& obj)
{
	switch(size_)
	{
	case 0:
		size_ = ds;
		ds *= 2;
		AllocArray();
		AddObj(obj);
		break;

	default:
		if(cur_size == size_)
		{
			ExpandArray();
		}

		AddObj(obj);
		break;
	}
}

// Remove object at index
template <class T>
void SciArray<T>::Pop(int index)
{
	assert(index >= 0 && index < cur_size);

	if(index == cur_size-1)
	{
		--cur_size;
	}
	else
	{
		T* new_data = new T[size_];
		T* after_element = data + index + 1;
		T* new_data_after = new_data + index;
		int after_size = cur_size - index + 1;
		memcpy(new_data, data, index*sizeof(T));
		memcpy(new_data_after, after_element, after_size*sizeof(T));

		CleanUp();

		--cur_size;
		data = new_data;
	}
}

// Type must have overloaded comparasion operators to be sorted
template <class T>
void SciArray<T>::Sort(bool low_to_high)
{
	if(low_to_high)
	{
		// Optimised Insertion Sort
		for(int i = 1; i < cur_size; ++i)
		{
			int j;
			T cur = data[i]; // Current element that is being tested
			// Shift sorted sub-array to the right, to make space for cur
			for(j = i-1; j >= 0 && data[j] > cur; --j)
			{
				data[j+1] = data[j];
			}
			// Insert current element into free spot
			data[j+1] = cur;
		}
	}
	else
	{
		for(int i = 1; i < cur_size; ++i)
		{
			int j;
			T cur = data[i];
			for(j = i-1; j >= 0 && data[j] < cur; --j)
			{
				data[j+1] = data[j];
			}
			data[j+1] = cur;
		}
	}
}

template <class T>
void SciArray<T>::Remove(const T& obj, bool first_inst)
{
	if(first_inst) // Remove 1st copy only
	{
		// Linear search
		for(int i = 0; i < cur_size; ++i)
		{
			if(data[i] == obj)
			{
				Pop(i);
				return;
			}
		}
	}
	else // Remove all copies
	{
		for(int i = 0; i < cur_size; ++i)
		{
			if(data[i] == obj)
			{
				Pop(i);
			}
		}
	}
}
// End of Data manipulation

// Private functions
template <class T>
void SciArray<T>::AllocArray()
{
	data = new T[size_];
}

template <class T>
void SciArray<T>::ExpandArray()
{
	int new_size = size_ + ds;
	// Double next array expantion
	ds *= 2;
	T* new_data = new T[new_size];

	// Copy data to new array
	memcpy(new_data, data, size_*sizeof(T));

	CleanUp();

	size_ = new_size;
	data = new_data;
}

template <class T>
void SciArray<T>::CleanUp()
{
	if(data)
	{
		delete[] data;
	}
}
// End of Private functions
// End of Template Implementation

#endif /* SCIARRAY_H */