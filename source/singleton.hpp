#ifndef SINGLETON_INCLUDED
#define SINGLETON_INCLUDED

template <typename T> 
class singleton
{
public:
    static T & instance()
    {
	    if(Instance == 0)
		    Instance = new T;
	    return *Instance;
    }

    static void destroy()
    {
	    if(Instance != 0)
	    {
		    delete Instance;
		    Instance = 0;
	    }
    }

private:
    static T* Instance;
};

template <typename T> T* singleton<T>::Instance = 0;

#endif //SINGLETON_INCLUDED
