#pragma once

#include <vector>

template <typename T>
class factory
{
public:
	typedef T value_type;
	typedef std::size_t size_type;

protected:
	std::vector<T*> Objects;

	factory(){}
	~factory();

public:
	T const *const operator[](size_type Index) const;
	size_type size() const;
};

template <typename T>
inline factory<T>::~factory()
{
	for
	(
		typename std::vector<T*>::iterator it = this->Objects.begin();
		it != this->Objects.end();
		it++
	)
	{
		delete *it;
	}
}

template <typename T>
inline T const *const factory<T>::operator[](size_type Index) const
{
	return this->Objects[Index];
}

template <typename T>
inline typename factory<T>::size_type factory<T>::size() const
{
	return this->Objects.size();
}
