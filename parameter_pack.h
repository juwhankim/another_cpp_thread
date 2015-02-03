#ifndef juwhan_parameter_pack_h
#define juwhan_parameter_pack_h

#include <cstdlib>
#include "juwhan_std.h"
#include "fast_function.h"


#define PARAM0
#define PARAM1 this->arg1
#define PARAM2 PARAM1, this->arg2
#define PARAM3 PARAM2, this->arg3
#define PARAM4 PARAM3, this->arg4
#define PARAM5 PARAM4, this->arg5
#define PARAM6 PARAM5, this->arg6
#define PARAM7 PARAM6, this->arg7
#define PARAM8 PARAM7, this->arg8
#define PARAM9 PARAM8, this->arg9
#define PARAM10 PARAM9, this->arg10
#define EXECUTE_MACRO(N) func(PARAM##N)

// This header file defines a variable length parameter pack class.
// The pack is usually included in a thread class.

namespace juwhan
{
	namespace std
	{

// Parameter pack.
template<size_t N, typename H, typename... T> struct parameter_pack_helper {};
#define P_P_H_MACRO(N) template<typename H, typename... T> struct parameter_pack_helper<N, H, T...> : parameter_pack_helper<N+1, T...> { using arg##N##_type = H; H arg##N; template<typename HH, typename... TT> parameter_pack_helper(HH&& head, TT&&... tail) : parameter_pack_helper<N+1, T...>{forward<TT>(tail)...}, arg##N(forward<HH>(head)) {}; };
P_P_H_MACRO(1); P_P_H_MACRO(2); P_P_H_MACRO(3); P_P_H_MACRO(4); P_P_H_MACRO(5); P_P_H_MACRO(6); P_P_H_MACRO(7); P_P_H_MACRO(8); P_P_H_MACRO(9); P_P_H_MACRO(10);
#undef P_P_H_MACRO
#define P_P_H_MACRO(N) template<typename H> struct parameter_pack_helper<N, H> { static constexpr size_t arity = N; using arg##N##_type = H; H arg##N; template<typename HH> parameter_pack_helper(HH&& head) : arg##N(forward<HH>(head)) {}; };
P_P_H_MACRO(1); P_P_H_MACRO(2); P_P_H_MACRO(3); P_P_H_MACRO(4); P_P_H_MACRO(5); P_P_H_MACRO(6); P_P_H_MACRO(7); P_P_H_MACRO(8); P_P_H_MACRO(9); P_P_H_MACRO(10);
#undef P_P_H_MACRO
template<typename F, typename... A>
struct parameter_pack : parameter_pack_helper<1, A...>
{
	using this_type = parameter_pack<F, A...>;
	using result_type = typename function_traits<F>::result_type;
	fast_function<F> func;
	// Use perfect forwarding in constructors.
	template<typename FF, typename... AA> parameter_pack(FF&& _func, AA&&... args)
	: parameter_pack_helper<1, A...>(forward<AA>(args)...), func(forward<FF>(_func)) {};
	// Operator().
#define OP_MACRO(N) template<typename V = this_type> typename enable_if<V::arity==N && !is_same<void, result_type>::value, result_type>::type operator()() { return EXECUTE_MACRO(N); };
OP_MACRO(1); OP_MACRO(2); OP_MACRO(3); OP_MACRO(4); OP_MACRO(5); OP_MACRO(6); OP_MACRO(7); OP_MACRO(8); OP_MACRO(9); OP_MACRO(10);
#undef OP_MACRO 
#define OP_MACRO(N) template<typename V = this_type> typename void_if<V::arity==N && is_same<void, result_type>::value>::type operator()() { EXECUTE_MACRO(N); };
OP_MACRO(1); OP_MACRO(2); OP_MACRO(3); OP_MACRO(4); OP_MACRO(5); OP_MACRO(6); OP_MACRO(7); OP_MACRO(8); OP_MACRO(9); OP_MACRO(10);
#undef OP_MACRO 
};
template<typename F>
struct parameter_pack<F>
{
	static constexpr size_t arity = 0;
	using this_type = parameter_pack<F>;
	using result_type = typename function_traits<F>::result_type;;
	fast_function<F> func;
	// Use perfect forwarding in constructors.
	template<typename FF> parameter_pack(FF&& _func) : func(forward<FF>(_func)) {};
	// Operator().
	template<typename V = this_type> 
	typename enable_if<!is_same<void, typename V::result_type>::value, result_type>::type operator()() 
	{
		return func(); 
	};
	template<typename V = this_type> 
	typename void_if<is_same<void, typename V::result_type>::value>::type operator()() 
	{
		func();
	};
}; 


template<typename F, typename... A> inline void* pack_parameters(F&& _func, A&&... args)
{
	// Use malloc to make sure the memory will be released in the form of void*.
	void* pack = malloc(sizeof(parameter_pack<typename remove_pointer<typename decay<F>::type>::type, typename decay<A>::type...>));
	new (pack) parameter_pack<typename remove_pointer<typename decay<F>::type>::type, typename decay<A>::type...>{forward<F>(_func), forward<A>(args)...};
	return pack;
};

template<typename F, typename... A> inline parameter_pack<typename remove_pointer<typename decay<F>::type>::type, typename decay<A>::type...>* unpack_parameters(void* pack)
{
	return reinterpret_cast<parameter_pack<typename remove_pointer<typename decay<F>::type>::type, typename decay<A>::type...>*>(pack);
};

	}  // End of namespace std.
}  // End of namespace juwhan.


// Undef
#undef PARAM0
#undef PARAM1
#undef PARAM2
#undef PARAM3
#undef PARAM4
#undef PARAM5
#undef PARAM6
#undef PARAM7
#undef PARAM8
#undef PARAM9
#undef PARAM10
#undef EXECUTE_MACRO


#endif










