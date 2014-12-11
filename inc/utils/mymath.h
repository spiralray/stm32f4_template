#pragma once

namespace MyMath {
	static const float PI = 3.1415927f;

	template<typename T> inline void saturate(T &x, T a){
		if(x >  a) x=a;
		if(x < -a) x=-a;
	}

	template<typename T> inline T saturated(T x, T a){
		T y=x; saturate(y,a);
		return y;
	}

	template<typename T> inline void normalize(T &x, T a){
		if(x>a){ x-=T(2)*a; }
		else if(x<=-a){ x+=T(2)*a; }
	}

	template<typename T> inline T normalized(T x, T a){
		T y=x; normalize(y,a);
		return y;
	}

	template<typename T> inline T squared(T x){
		return x*x;
	}

	template<typename T> int8_t signof(T val){
		return (T(0)<val)-(val<T(0));
	}
}
