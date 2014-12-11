#include "../utils/mymath.h"

template<typename T>
class PIDController {
public:
	PIDController(T _Kp, T _Ki, T _Kd, T _isat=T(0)):Kp(_Kp),Ki(_Ki),Kd(_Kd),isat(_isat),ep(T(0)),ei(T(0)),u(T(0)){}
	~PIDController(){}
	void update(T r, T x){
		T e=r-x; T ed=e-ep; ei+=e; ep=e;
		if(isat>T(0)){ MyMath::saturate(ei, isat); } // Anti wind-up
		u=Kp*e+Ki*ei+Kd*ed;
	}
	T getOutput() const { return u; }

	void setKp(T _Kp){
	  Kp = _Kp;
	}
protected:
	T Kp;
	T Ki;
	T Kd;
	T isat;
	T ep;
	T ei;
	T u;
};
