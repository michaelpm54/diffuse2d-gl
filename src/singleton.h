#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
	public:
		Singleton(Singleton const&) = delete;
		void operator=(Singleton const&) = delete;

		static Singleton& instance()
		{
			static Singleton _instance;
			return _instance;
		}

	private:
		Singleton() {}
};

#endif // SINGLETON_H
