#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "MyVector.h"

using namespace std;

/****************************************************************************/
//Дефолтный конструктор
MyVector::MyVector(size_t size, ResizeStrategy resizeStrategy, float coef)
{
	_size = size;
	_coef = coef;
	_resStrategy = resizeStrategy;
	_capacity = (float)sizeof(ValueType) * coef;
	_data = new ValueType[_capacity];
}

MyVector::MyVector(size_t size, ValueType value, ResizeStrategy resizeStrategy, float coef)
{
	_size = size;
	_coef = coef;
	_resStrategy = resizeStrategy;
	if (size != 0)
		_capacity = (float)sizeof(ValueType) * coef;

	_data = new ValueType[_capacity];
	for (size_t i = 0; i < size; ++i) {
		_data[i] = value;
	}
}

MyVector::MyVector(const MyVector& copy)
{
	_size = copy._size;
	_coef = copy._coef;
	_resStrategy = copy._resStrategy;
	_capacity = copy._capacity;
	_data = new ValueType[_capacity];
	memcpy(this->_data, copy._data, this->_size * sizeof(ValueType));
}

MyVector& MyVector::operator=(const MyVector& copy)
{
	if (this == &copy) return *this;

	ValueType* tempData = new ValueType[copy._capacity];
	memcpy(tempData, copy._data, copy._size * sizeof(ValueType));
	delete[] this->_data;

	_data = tempData;
	_size = copy._size;
	_coef = copy._coef;
	_resStrategy = copy._resStrategy;
	_capacity = copy._capacity;

	return *this;
}

MyVector::MyVector(MyVector&& moveVector) noexcept
{
	_size = moveVector._size;
	_coef = moveVector._coef;
	_resStrategy = moveVector._resStrategy;
	_capacity = moveVector._capacity;
	_data = new ValueType[_capacity];
	memcpy(this->_data, moveVector._data, this->_size * sizeof(ValueType));

	moveVector._size = 0;
	moveVector._capacity = 0;
	moveVector._coef = 0;
	moveVector._data = nullptr;
}

MyVector& MyVector::operator=(MyVector&& moveVector) noexcept
{
	if (this == &moveVector) return *this;

	delete[] this->_data;

	_data = moveVector._data;
	_size = moveVector._size;
	_coef = moveVector._coef;
	_resStrategy = moveVector._resStrategy;
	_capacity = moveVector._capacity;

	moveVector._size = 0;
	moveVector._capacity = 0;
	moveVector._coef = 0;
	moveVector._data = nullptr;
}

MyVector::~MyVector()
{
	delete[] this->_data;
}

/**********************************************************************************/


size_t MyVector::capacity() const
{
	return _capacity;
}

size_t MyVector::size() const
{
	return _size;
}

float MyVector::loadFactor()
{
	return (float) _size / (float)_capacity;
}

void MyVector::reserve(const size_t capacity)
{
	if (capacity < _capacity) return;

	if (_data == nullptr) {
		_data = new ValueType[capacity];
	}
	else {
		ValueType* temp = _data;
		_data = new ValueType[capacity];
		memcpy(_data, temp, _size * sizeof(ValueType));
		delete temp;
	}
}

void MyVector::strategyReserve()
{
	if (this->loadFactor() >= 1) {
		if (_resStrategy == ResizeStrategy::Multiplicative)
			reserve(_capacity * _coef);
		if (_resStrategy == ResizeStrategy::Additive)
			reserve(_capacity + _coef);
	}
}

ValueType& MyVector::operator[](const size_t i) const
{
	if (i < 0 or i > _size) {
		assert(i < 0 or i > _size);
	}
	return _data[i];
}

void MyVector::pushBack(const ValueType& value)
{
	if (_size == 0) {
		_data[_size] = value;
	}
	/*else if (this->loadFactor() <= 1) {
		_data[_size + 1] = value;
	}
	else {
		this->reserve(_size + 1);
		_data[_size + 1] = value;
	}*/
	else this->strategyReserve();
	_size++;
}

void MyVector::insert(const size_t i, const ValueType& value)
{
	if (i <= _capacity) {
		_data[i] = value;
		return;
	}

	if (i == _size) {
		pushBack(value);
		return;
	}

	this->strategyReserve();

	/*size_t oldCap = _capacity;
	if (i > _capacity) {
		while (_capacity < i) {
			_capacity = (float)_capacity * _coef;
		}
		ValueType* buff = new ValueType[oldCap];
		memcpy(buff, _data, _size * oldCap);
		buff = _data;

		delete[] _data;
		_data = new ValueType[_capacity];
		memcpy(_data, buff, _capacity);
		_data[i] = value;*/

	for (size_t j = _size; j > i; j--) {
		_data[j] = _data[j - 1];
	}
	_data[i] = value;
		
	_size++;
}

void MyVector::insert(const size_t i, const MyVector& value)
{
	if (value._size + this->_size < this->_capacity && i < this->_capacity) {
		for (size_t k = 0; k < value._size; ++k) {
			_data[k + i] = value._data[k];
		}
	}
	else {
		this->reserve(_capacity * _coef);
		for (size_t k = _size + value._size; k > i + value._size; k--)
		{
			this->_data[k] = this->_data[k - value._size];
		}
		for (size_t k = i; k < i + value._size; k++)
		{
			this->_data[k] = value._data[k - i];
		}
	}
	_size += value._size;
}

void MyVector::popBack()
{
	if (_data == nullptr)
		return;
	--_size;
}

void MyVector::erase(const size_t i)
{
	if (this->_data == nullptr)
		return;

	if (i == _size) {
		this->popBack();
	}
	else {
		for (size_t j = i; j < _size - 1; j++) {
			_data[j] = _data[j + 1];
		}
		--_size;
	}
}

void MyVector::erase(const size_t i, const size_t len)
{
	if (i and len > _size or i and len < _size) {
		return;
	}

	for (size_t k = i; k < size() - len; k++)
	{
		this->_data[k] = this->_data[k + len];
	}
	_size += -len;
}

long long int MyVector::find(const ValueType &value, bool isBegin) const
{
	if (_size == 0)
		return -1;

	if (isBegin == true)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (this->_data[i] == value)
				return  i;
		}
	}
	else
	{
		for (size_t i = _size - 1; i >= 0; --i)
		{
			if (this->_data[i] == value)
				return i;
		}

	}
	return -1;
}

void MyVector::resize(const size_t size, const ValueType value)
{
	if (size > _capacity)
	{
		reserve(size);
		for (size_t i = _size; i < size; i++)
		{
			_data[i] = value;
		}
		_size = size;
	}

	if (size == _capacity)
		return;

	if (size < _capacity)
	{
		reserve(size);
		_size = size;
	}
	return;
}

void MyVector::clear()
{
	delete[] _data;
	_data = nullptr;
	_size = 0;
}


