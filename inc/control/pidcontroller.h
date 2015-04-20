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
	T getKp(){
		return Kp;
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

template<typename T>
class PIDSpeedController: public PIDController<T> {
public:
	PIDSpeedController(T _Kp, T _Ki, T _Kd, T _isat=T(0), T _dt=T(1), T _maxPower=T(1)) : PIDController<T>(_Kp,_Ki,_Kd)
	{
		maxPower = _maxPower;
	}

	~PIDSpeedController(){}
	void update(T r, T x){
		T deff = r - x;
		T old_u = PIDController<T>::u;

		PIDController<T>::ei += (deff + ed)/2.0 * dt;
		if(PIDController<T>::isat>T(0)){ MyMath::saturate(PIDController<T>::ei, PIDController<T>::isat); } // Anti wind-up

		T ret = PIDController<T>::Kp * deff + PIDController<T>::Ki * PIDController<T>::ei + PIDController<T>::Kd * (deff - ed) / dt;

		ed = deff;
		PIDController<T>::u = old_u + ret;

		if( PIDController<T>::u > maxPower ) PIDController<T>::u = maxPower;
		else if( PIDController<T>::u < -maxPower ) PIDController<T>::u = -maxPower;

	}
protected:
	T dt;
	T ed;
	T maxPower;
};