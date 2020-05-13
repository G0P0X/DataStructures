#include "MyVector.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

MyVector::MyVector(size_t size = 0, ResizeStrategy resizestrategy, float coef)
{
	_size = size;
	_resizestrategy = resizestrategy;
	_coef = coef;

	if (_resizestrategy == ResizeStrategy::Multiplicative) {
		if (_size == 0) {
			_capacity = 1;
		}
		else {
			_capacity = _size * _coef;
		}
		_data = new ValueType[_capacity];
	}
	else {
		_capacity = _size + 3;
	}
	_data = new ValueType[_capacity];
}

MyVector::MyVector(size_t size, ValueType value, ResizeStrategy resizestrategy, float coef)
{
	_size = size;
	_resizestrategy = resizestrategy;
	_coef = coef;

	if (_resizestrategy == ResizeStrategy::Multiplicative) {
		if (_size == 0) {
			_capacity = 1;
		}
		else {
			_capacity = _size * _coef;
		}
		_data = new ValueType[_capacity];
	}
	else {
		_capacity = _size + 3;
	}
	_data = new ValueType[_capacity];

	_data[0] = value;
}

MyVector::MyVector(const MyVector& copy)
{
	this->_size = copy._size;
	this->_capacity = copy._capacity;
	this->_resizestrategy = copy._resizestrategy;
	this->_data = new ValueType[copy._capacity];
	for (size_t i = 0; i < _size; i++) {
		_data[i] = copy._data[i];
	}
}

MyVector& MyVector::operator=(const MyVector& copy)
{
	if (this == &copy) {
		return *this;
	}

	delete[] _data;
	_size = copy._size;
	_capacity = copy._capacity;
	_data = new ValueType[_capacity];
	for (size_t i = 0; i < _size; i++) {
		_data[i] = copy._data[i];
	}

	return *this;
}

MyVector::~MyVector()
{
	delete[] _data;
}

size_t MyVector::capacity() const
{
	return _capacity;
}

size_t MyVector::size() const
{
	return _size;
}