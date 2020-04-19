#include "LinkedList.h"
#include <cassert>

using namespace std;

// Конструктор узла для начала создания списка и создания новых узлов 
// принимаем значение и записываем его в узел, указатель узла содержить нулевой указатель
// конец списка определяется узлом nullptr

LinkedList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LinkedList::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}


void LinkedList::Node::insertNext(const ValueType& value)
{
	Node* newNode = new Node(value, this->next);
	this->next = newNode;
}

void LinkedList::Node::removeNext()
{
	Node* remNext = this->next;
	Node* newNext = remNext->next;
	delete remNext;
	this->next = newNext;
}

////////////////////////////////////

// Дефолтный конструктор
LinkedList::LinkedList() 
{
	_head = nullptr;
	_size = 0;
}

// Правило пяти												
// Конструктор копирования
LinkedList::LinkedList(const LinkedList& copyList)
{
	this->_size = copyList._size;
	if (this->_size == 0) {
		_head = nullptr;
		return;
	}

	this->_head = new Node(copyList._head->value);

	Node* currentNode = this->_head;
	Node* currentCopyNode = copyList._head;

	while (currentCopyNode->next) {
		currentNode->next = new Node(currentCopyNode->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	}

}

// Оператор присваивания копированием						 
LinkedList& LinkedList::operator=(const LinkedList& copyList)
{
	//Ето проверка на самоприсваивание
	if (this == &copyList) {
		return *this;
	}

	LinkedList buffer(copyList);
	this->_size = buffer._size;
	this->_head = buffer._head;

	return *this;
}

// Конструктор перемещения
LinkedList::LinkedList(LinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;
}

//Оператор присваивания перемещением
LinkedList& LinkedList::operator=(LinkedList&& moveList) noexcept
{
	if (this == &moveList) {
		return *this;
	}

	forceNodeDelete(_head);
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

	return *this;
}

LinkedList::~LinkedList()
{
	forceNodeDelete(_head);
}

//////////////////////////////

void LinkedList::forceNodeDelete(Node* node)
{
	if (node == nullptr) {
		return;
	}

	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);
}

// доступ к значению элемента по индексу
ValueType& LinkedList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

LinkedList::Node* LinkedList::getNode(const size_t pos) const
{
	if (pos < 0) {
		assert(pos < 0);
	}
	else if (pos >= this->_size) {
		assert(pos >= this->size);
	}

	Node* gettingNode = this->_head;
	for (size_t i = 0; i < pos; ++i) {
		gettingNode = gettingNode->next;
	}

	return gettingNode;
}


void LinkedList::insert(const size_t pos, const ValueType& value)
{
	if (pos < 0) {
		assert(pos < 0);
	}
	else if (pos > this->_size) {
		assert(pos > this->_size);
	}

	if (pos == 0) {
		pushFront(value);
	}
	else {
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i) {
			bufNode = bufNode->next;
		}
		bufNode->insertNext(value);
		++_size;
	}
}

void LinkedList::insertAfterNode(Node* node, const ValueType& value)
{
	node->insertNext(value);
}

void LinkedList::pushBack(const ValueType& value)
{
	if (_size == 0) {
		pushFront(value);
		return;
	}
	insert(_size, value);
}

void LinkedList::pushFront(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
}


// Удаление
void LinkedList::remove(const size_t pos)
{
	if (pos == 0) {
		removeFront();
	}
	else {
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i) {
			bufNode = bufNode->next;
			Node* removeNode = bufNode->next;
			bufNode->next = removeNode->next;
			delete removeNode;
			_size--;
		}
	}
}

void LinkedList::removeNextNode(Node* node)
{
	if (node->next == nullptr) {
		return;
	}
	else {
		node->removeNext();
	}
}

void LinkedList::removeFront()
{
	Node* removeNode = this->_head;
	this->_head = _head->next;

	delete removeNode;
	_size--;
}

void LinkedList::removeBack()
{
	remove(_size - 1);
}

size_t LinkedList::size() const
{
	return _size;
}

long long int LinkedList::findIndex(const ValueType& value) const
{
	Node* bufNode = this->_head;
	bool flag = 0;
	for (int i = 0; i < _size; ++i) {
		if (bufNode->value == value) {
			flag = 1;
			return i;
		}
		else bufNode = bufNode->next;
	}

	if (flag == 0) return -1;
}

LinkedList::Node* LinkedList::findNode(const ValueType& value) const
{
	Node* bufNode = this->_head;
	bool flag = 0;
	for (size_t i = 0; i < _size; ++i)
	{
		if (value == bufNode->value) {
			flag = 1;
			return bufNode;
		}
		else {
			bufNode = bufNode->next;
		}
	}

	if (flag == 0) return nullptr;
}

// разворот списка

void LinkedList::reverse()
{
	Node* reverseNode = nullptr;

	while (this->_head != nullptr)
	{
		Node* temp = _head->next;
		_head->next = reverseNode;
		reverseNode = _head;
		_head = temp;
	}
	_head = reverseNode;
}

LinkedList LinkedList::reverse() const
{
	LinkedList reversed = (*this);
	reversed.reverse();

	return reversed;
}

LinkedList LinkedList::getReverseList() const
{
	LinkedList reversed = (*this);
	reversed.reverse();

	return reversed;
}