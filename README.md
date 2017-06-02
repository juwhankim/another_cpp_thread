# another_cpp_thread
A c++11 thread class implemented using pthread

The C++11 standard library thread is easy to use, and beautifully designed.

However, it sometimes is not interoperable with the good old pthread functions. For example, on the platform I was working on get/setkey did not work with the C++11 thread and resulted in runtime error. The reason why I wanted to use get/setkey was because the platform, OSX specifically did not support threadlocal types.

Also, not all features defined in C++11 are supported in every platform and to make things worse - the unsupported feature sets are quite unpredictable varying over platforms.

Hence, I reimplemented basic(mostly type_traits) C++ library functions to minimize dependency on the C++ template library and packaged them with the thread library.

If you're having trouble with portability or interoperability with the standard C++ thread library, try using this implementation.
