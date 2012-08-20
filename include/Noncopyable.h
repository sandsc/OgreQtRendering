#pragma once

namespace sandgis
{
	class Noncopyable
	{
	protected:
		Noncopyable() {}
		~Noncopyable() {}
	private:
		Noncopyable& operator=(const Noncopyable&);
		Noncopyable(const Noncopyable&);
	};
}