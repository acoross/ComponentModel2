#pragma once

#include <cereal\archives\binary.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\string.hpp>
#include <strstream>
#include "Numerics.h"

namespace scl
{
	class _PacketBuffer
	{
	public:
		_PacketBuffer(ulong size)
			: buf(new char[size]), size(size), filled(0)
		{
			
		}

		~_PacketBuffer()
		{
			delete[] buf;
		}

		char* buf;
		ulong size;
		ulong filled;
	};

	typedef std::shared_ptr<_PacketBuffer> PacketBuffer;

	class PacketWriter
	{
	public:
		PacketWriter()
			: _buf(std::make_shared<_PacketBuffer>(1024 * 4)), _stream(_buf->buf, _buf->size), _ar(_stream)
		{}

		template <class T>
		PacketBuffer Write(const T& val)
		{
			_ar((uint32)0);	//frame
			_ar(val.mid);	//type
			_ar(val);		//msg
			_buf->filled += _stream.tellp();

			*(uint32*)_buf->buf = (uint32)_buf->filled;
			return _buf;
		}
		
		PacketBuffer _buf;
		std::ostrstream _stream;
		cereal::BinaryOutputArchive _ar;
	};

	class PacketReader
	{
	public:
		PacketReader(char* buf, ulong siz)
			: _siz(siz), _stream(buf, siz), _ar(_stream)
		{}

		bool ReadHeader(uint32& packetLen, uint32& type)
		{
			if (_siz < sizeof(uint32))
				return false;
			
			_ar(packetLen);
			if (_siz < packetLen)
				return false;

			_ar(type);
			return true;
		}

		template <class T>
		void ReadBody(T& val)
		{
			_ar(val);
		}

		const ulong _siz;
		std::istrstream _stream;
		cereal::BinaryInputArchive _ar;
	};
}