#ifndef juwhan_threadlocal_h
#define juwhan_threadlocal_h


#include <system_error>
#include <iostream>

namespace juwhan
{
	namespace threadpool
	{

using namespace juwhan::std;

template<typename T>
class threadlocal
{
  pthread_key_t key;
  bool is_valid;
public:
  threadlocal() : is_valid{true}
  {
    if (pthread_key_create(&key, NULL))
    {
      is_valid = false;
      throw ::std::system_error{::std::error_code{}, "Key generation for thread specific storage failed."};
    }
  };
  threadlocal(threadlocal& other) : key{other.key}, is_valid{other.is_valid} {};
  threadlocal(threadlocal&& other) : key{other.key}, is_valid{other.is_valid}
  {
    other.is_valid = false;
  }
  threadlocal& operator=(threadlocal& other)
  {
  	key = other.key;
  	is_valid = other.is_valid;
  }
  threadlocal& operator=(threadlocal&& other)
  {
    key = other.key;
    is_valid = other.is_valid;
    other.is_valid = false;
    return *this;
  }
  threadlocal& operator=(T& _value)
  {
    if (!is_valid) throw ::std::runtime_error{"You are trying to set value of an invalid thread local variable. Perhaps the the content has been moved to another variable."};
    T* value = reinterpret_cast<T*>(pthread_getspecific(key));
    if (value)
    {
      *value = _value;
    }
    else
    {
      value = reinterpret_cast<T*>(malloc(sizeof(T)));
      if (!value) throw ::std::system_error{::std::error_code{}, "Memory acquisition for thread specific storage failed."};
      *value = _value; 
      pthread_setspecific(key, value);
    }
    return *this;
  };
  threadlocal& operator=(T&& _value)
  {
    if (!is_valid) throw ::std::runtime_error{"You are trying to set value of an invalid thread local variable. Perhaps the the content has been moved to another variable."};
    T* value = reinterpret_cast<T*>(pthread_getspecific(key));
    if (value)
    {
      *value = move(_value);
    }
    else
    {
      value = reinterpret_cast<T*>(malloc(sizeof(T)));
      if (!value) throw ::std::system_error{::std::error_code{}, "Memory acquisition for thread specific storage failed."};
      *value = move(_value); 
      pthread_setspecific(key, value);
    }
    return *this;
  };
  operator T() const
  {
    if (!is_valid) throw ::std::runtime_error{"You are trying to access value of an invalid thread local variable. Perhaps the the content has been moved ."};
    T* value = reinterpret_cast<T*>(pthread_getspecific(key));
    if (!value) throw ::std::runtime_error{"You are trying access to value of an unset thread local variable. You must set it first."};
    return *value;
  };
  operator T*() const
  {
    if (!is_valid) throw ::std::runtime_error{"You are trying to access value of an invalid thread local variable. Perhaps the the content has been moved ."};
    T* value = reinterpret_cast<T*>(pthread_getspecific(key));
    if (!value) throw ::std::runtime_error{"You are trying access to value of an unset thread local variable. You must set it first."};
    return value;
  };
  ~threadlocal()
  {
  	 T* value = reinterpret_cast<T*>(pthread_getspecific(key));
  	 if (value && is_valid) free(value);
  }
};

template<typename CharT, typename Traits, typename T> inline ::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& ost, threadlocal<T>& tl)
{
  return ost << static_cast<T>(tl);
};




	}  // End of namespace std.
}  // End of namespace juwhan.



#endif