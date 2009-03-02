/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: singleton.h
 * Description:
        This file is to define FastQueue.
 * Author: cch
 * Update: 2009-2-21
 * Verison: 0.0.1
=============================================================================*/


#ifndef FAST_QUEUE_H_
#define FAST_QUEUE_H_

#include "condition.h"
#include "mutex.h"

namespace srdgame
{

template <typename T>
class FastQueue
{
	struct Node
	{
		Node(T& item) : val(item), next(NULL)
		{}
		T val;
		Node* next;
	};
public:
	inline FastQueue() : _cond(_mutex), _first(NULL), _last(NULL), _size(0)
	{
	}
	inline ~FastQueue()
	{
		
	}
	int get_size()
	{
		_cond.lock();
		int size = _size;
		_cond.unlock();
		return size;
	}
	void push(T& item)
	{
		Node* node = new Node(item);
		_cond.lock();
		if (_last)
		{
			_last->next = node;
			_last = node;
			++_size;
		}
		else
		{
			_last = _first = node;
			_cond.signal();
			_size = 1;
		}
		_cond.unlock();
	}
	bool try_pop(T& val)
	{
		_cond.lock();
		if (0 == _size || _first == NULL)
		{
			_cond.unlock();
			return false;
		}
		val = _first->val;
		delete _first;
		if (--_size)
		{
			_first = _last = NULL;
		}
		else
		{
			_first = _first->next;
		}
		_cond.unlock();
		return true;
	}
	T& try_pop()
	{
		T val;
		if (try_pop(val))
		{
			return val;
		}
		else
		{
			return NULL;
		}
	}
	bool pop(T& val)
	{
		_cond.lock();
		if (0 == _size || _first == NULL)
		{
			_cond.wait();
		}
		val = _first->val;
		delete _first;
		if (--_size)
		{
			_first = _first->next;
		}
		else
		{
			_first = _last = NULL;
		}
		_cond.unlock();
	}
	T& pop()
	{
		T val;
		if (pop(&val))
		{
			return val;
		}
		else
		{
			return NULL;
		}
	}
	inline Condition& get_cond()
	{
		return _cond;
	}

	inline void clear()
	{
		_cond.lock();
		if (_size)
		{
			Node* tmp = _first;
			while (_first)
			{
				_first = _first->next;
				delete tmp;
				tmp = _first;
			}
		}
		_cond.unlock();
	}

private:
	Mutex _mutex;
	Condition _cond;
	Node* _first;
	Node* _last;
	volatile unsigned int _size;

};
}

#endif
