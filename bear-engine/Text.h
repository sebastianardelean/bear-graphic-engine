#pragma once
#if defined(_WIN32) || defined(WIN32) 
#ifdef BEARENGINE_EXPORTS
#define BEARENGINE_API __declspec(dllexport)
#else
#define BEARENGINE_API __declspec(dllimport)
#endif
#elif defined(__unix__)
#define BEARENGINE_API 
#endif // _WIN32

namespace bear {
	class Text
	{
	public:
	protected:
	private:
	};
};


