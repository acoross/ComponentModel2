#pragma once

#include <string>
#include "Numerics.h"
#include <map>
#include <sstream>

namespace CSProtocol
{
	using namespace scl;

	struct FVector
	{
		FVector()
			: X(0), Y(0), Z(0)
		{}

		FVector(float x, float y, float z)
			: X(x), Y(y), Z(z)
		{}

		float X;
		float Y;
		float Z;

		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(X, Y, Z);
		}

		std::wstring ToString() const
		{
			std::wstringstream ss;
			ss << "(" << X << ", " << Y << ", " << Z << ")";
			return ss.str();
		}
	};

	namespace CS
	{
		enum class Protocols : uint32
		{
			RequestLogin = 0,
			Move,
			MAX
		};

		struct RequestLogin
		{
			static const uint32 mid = (uint32)Protocols::RequestLogin;

			std::wstring id;
			std::wstring password;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(id, password);
			}
		};

		struct Move
		{
			static const uint32 mid = (uint32)Protocols::Move;

			FVector position;
			FVector velocity;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(position, velocity);
			}
		};

		/*template <Protocols T>
		class Protocol_t
		{};

		template <> struct Protocol_t<Protocols::RequestLogin> { typedef RequestLogin T; };
		template <> struct Protocol_t<Protocols::Move> { typedef Move T; };*/
	}

	namespace SC
	{
		enum class Protocols : uint32
		{
			ResponseLogin = 0,
			NotiMove,
			NotiEnterZone,
			NotiLeaveZone,
			MAX
		};

		struct ResponseLogin
		{
			static const uint32 mid = (uint32)Protocols::ResponseLogin;

			bool ok;
			uint64 gameObjectId;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(ok, gameObjectid);
			}
		};

		struct NotiMove
		{
			static const uint32 mid = (uint32)Protocols::NotiMove;

			uint64 gameObjectId;
			FVector position;
			FVector velocity;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(gameObjectid, position, velocity);
			}
		};

		struct NotiEnterZone
		{
			static const uint32 mid = (uint32)Protocols::NotiEnterZone;

			uint64 gameObjectId;
			FVector position;
			FVector velocity;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(gameObjectid, position, velocity);
			}
		};

		struct NotiLeaveZone
		{
			static const uint32 mid = (uint32)Protocols::NotiLeaveZone;

			uint64 gameObjectId;

			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(gameObjectId);
			}
		};
	}
}