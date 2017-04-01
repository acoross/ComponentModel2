#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <strstream>
#include "scl/Types.h"

namespace scl
{
	typedef int32 psizT;

	class _PacketBuffer
	{
	public:
		_PacketBuffer(psizT size)
			: buf(new char[size]), size(size), filled(0)
		{
			
		}

		~_PacketBuffer()
		{
			delete[] buf;
		}

		char* buf;
		psizT size;
		psizT filled;
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
			_ar((psizT)0);	//frame
			_ar(val.PacketId);	//type
			_ar(val);		//msg
			_buf->filled += _stream.tellp();

			*(psizT*)_buf->buf = (psizT)_buf->filled;
			return _buf;
		}
		
		PacketBuffer _buf;
		std::ostrstream _stream;
		cereal::BinaryOutputArchive _ar;
	};

	class PacketReader
	{
	public:
		PacketReader(char* buf, psizT siz)
			: _siz(siz), _stream(buf, siz), _ar(_stream)
		{}

		bool ReadHeader(psizT& packetLen, psizT& type)
		{
			if (_siz < sizeof(psizT))
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

		const psizT _siz;
		std::istrstream _stream;
		cereal::BinaryInputArchive _ar;
	};
}