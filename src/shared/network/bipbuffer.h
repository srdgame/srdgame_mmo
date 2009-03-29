/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: bipbuffer.h
 * Description:
        This file is to define the bipbuf. Based on the Bip Buffer concept, from http://www.codeproject.com/KB/IP/bipbuffer.aspx
 * Author: cch
 * Update: 2009-2-23
 * Verison: 0.0.1
=============================================================================*/

#ifndef BIP_BUFFER_H_
#define BIP_BUFFER_H_

#include "bufferbase.h"

// Define debug
#define _BIP_DEBUG_

#ifdef _BIP_DEBUG_
#include "log.h"
#endif

#ifndef SYS_PAGE_SIZE
#define SYS_PAGE_SIZE 4096
#endif

using std::size_t;

namespace srdgame
{
class BipBuffer : public BufferBase
{
public:
	inline BipBuffer(size_t buf_size = SYS_PAGE_SIZE) : BufferBase(), buf(NULL), len(buf_size), ixa(0), sza(0), ixb(0), szb(0), ixr(0), szr(0)
	{
		// 分配页面整数倍的内存
		len = ((len + SYS_PAGE_SIZE - 1) / SYS_PAGE_SIZE ) * SYS_PAGE_SIZE;
		buf = new char[buf_size];
#ifdef _BIP_DEBUG_
		LogDebug("BipBuffer", "Buffer SIZE IS: %d", len);
#endif
		// 如果分配失败则设置为空
		if (buf == NULL)
		{
#ifdef _BIP_DEBUG_
			LogError("BipBuffer", "Could not allocate buffer for size : %d", len);
#endif
			len = 0;
		}
	}
	inline ~BipBuffer()
	{
		// 释放内存
		if (buf)
			delete[] buf;
	}

	inline void clear()
	{
		// 清楚所有信息
		ixa = sza = 0;
		ixb = szb = 0;
		ixr = szr = 0;
	}

	inline char* reserve(size_t size, size_t& reserved)
	{
#ifdef _BIP_DEBUG_
		LogDebug("BipBuffer", "Reserve size : %d", size);
#endif
		if (size == 0)
			return NULL;
		if (szb)
		{
#ifdef _BIP_DEBUG_
			LogDebug("BipBuffer", "Now using B region");
#endif
			// 当B部分存在的时候只从B部分后面分配数据区
			size_t free = get_b_free();
			if (size < free)
				free = size;
			if (free == 0)
				return NULL;
			szr = free;
			reserved = free;
			ixr = ixb + szb;
			return buf + ixr;
		}
		else
		{
#ifdef _BIP_DEBUG_
			LogDebug("BipBuffer", "Only using A region");
#endif
			// 当只有A部分的时候
			size_t free = get_a_free();
			if (free > size)
			{
				// 当A区剩余区域够用
#ifdef _BIP_DEBUG_
				LogDebug("BipBuffer", "Region A is enough to buf");
#endif
				szr = size;
				reserved = size;
				ixr = ixa + sza;
				return buf + ixr;
			}
			else if (free > ixa)
			{
#ifdef _BIP_DEBUG_
				LogDebug("BipBuffer", "Region A has bigger ares");
#endif
				// 当A区剩余比较大
				reserved = free;
				szr = free;
				ixr = ixa + sza;
				return buf + ixr;
			}
			else
			{
#ifdef _BIP_DEBUG_
				LogDebug("BipBuffer", "Create Region B");
#endif
				if (ixa == 0)
				{
					ixr = 0;
					szr = 0;
					reserved = 0;
					return NULL;
				}

				// 获取最小
				if (ixa < size)
					size = ixa;
				szr = size;
				reserved = size;
				ixr = 0;
				return buf;
			}
		}
	}
	inline void commit(size_t size)
	{
#ifdef _BIP_DEBUG_
		LogDebug("BipBuffer", "Commiting size : %d", size);
#endif
		if (size == 0)
		{
			ixr = szr = 0;
			return;
		}
		if (size > szr)
		{
			size = szr;
		}
		if (sza == 0 && szb == 0)
		{
			//两个区域都没有数据
			ixa = ixr;
			sza = size;
			ixr = szr = 0;
#ifdef _BIP_DEBUG_
			LogDebug("BipBuffer", "ixa : %d\t sza : %d", ixa, sza);
#endif
			return;
		}
		if (ixr == ixa + sza)
		{
			// 数据追加在A区之后
			sza += size;
		}
		else
		{
			// B区一直都在最开始的区域？
			// 数据追加在B区之后或者就是新的B区
			szb += size;
		}
		ixr = szr = 0;
#ifdef _BIP_DEBUG_
		LogDebug("BipBuffer", "Commit finished");
#endif
	}
	inline char* get_data(size_t& size)
	{
		if (sza == 0)
		{
#ifdef _BIP_DEBUG_
			LogDebug("BipBuffer", "No data avaliable");
#endif
			size = 0;
			return 0;
		}
#ifdef _BIP_DEBUG_
		LogDebug("BipBuffer", "Avaliable size : %d", sza);
#endif
		size = sza;
		return buf + ixa;
	}
	inline void free(size_t size)
	{
		if (size >= sza)
		{
			//将B区改为A区
			ixa = ixb;
			sza = szb;
			ixb = szb = 0;
		}
		else
		{
			//将A区减小
			sza -= size;
			ixa += size;
		}
	}
	inline size_t spare_size()
	{
		return max(get_a_free(), get_b_free());
	}
	inline size_t reserved_size()
	{
		return szr;
	}
	inline size_t buf_size()
	{
		return len;
	}
	inline size_t data_size()
	{
		return sza + szb;
	}
protected:
	size_t get_a_free()
	{
		return len - ixa - sza;
	}
	size_t get_b_free()
	{
		return ixa - ixb -szb;
	}
private:
	char* buf;
	size_t len;
	size_t ixa;
	size_t sza;
	size_t ixb;
	size_t szb;
	size_t ixr;
	size_t szr;
};
}

#endif
