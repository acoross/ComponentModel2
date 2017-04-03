#pragma once

namespace devs
{
	class DDMessage1
	{
	public:
	};

	class DDMessage2
	{
	public:
	};

	//
	struct DDMessageHandler
	{
		template <class TMsg>
		void Handle(const TMsg& msg) {}
		virtual void Handle(const DDMessage1& msg) {}
		virtual void Handle(const DDMessage2& msg) {}
	};

	class IDDComponent
	{
	public:
		virtual ~IDDComponent() {}
	};

	class DDComponent1 : public IDDComponent, public DDMessageHandler
	{
	public:
		virtual ~DDComponent1() {}

		virtual void Handle(const DDMessage1& msg)
		{
			// do something!
		}
	};

	template <class TMsg>
	void Dispatch(const TMsg& msg, DDMessageHandler* handler)
	{
		handler->Handle(msg);
	}

	// usage
	inline void sample()
	{
		DDComponent1 comp;
		Dispatch(1, &comp);
		Dispatch(DDMessage1(), &comp);
	}
}