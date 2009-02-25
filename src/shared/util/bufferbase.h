/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: bufferbase.h
 * Description:
        This file is to define buffer class.
 * Author: cch
 * Update: 2009-2-23
 * Verison: 0.0.1
=============================================================================*/

#ifndef BUFFER_BASE_H_
#define BUFFER_BASE_H_
namespace srdgame
{
class BufferBase
{
public:
	BufferBase(){}
	virtual BufferBase(){}

	virtual void clear() = 0;

	virtual char* reserve(size_t size, size_t& reserved) = 0;
	virtual void commit(size_t size) = 0;

	virtual char* get_data(size_t& size) = 0;
	virtual void free(size_t size) = 0;

	virtual size_t reserved_size() = 0;
	virtual size_t buf_size() = 0;
	virtual size_t data_size() = 0;
}

#endif
