#include "pidcontroller.h"

using namespace stm32plus;

template<typename T, class TMotor, class TEncoder>
class MotorController : public TMotor, public TEncoder {
public:
	MotorController(T Kp,T Ki,T Kd,T isat=0):TMotor(),TEncoder(),pid(Kp,Ki,Kd,isat){}
	~MotorController(){}
	void update(T r){
		// NOTE: should be called after captureSpeed
		int16_t x=TEncoder::getSpeed();
		pid.update(r,x);
		TMotor::setOutput(pid.getOutput());
	}
//protected:
	PIDController<T> pid;
};

template<typename T, class TMotor, class TEncoder>
class MotorPositionController : public MotorController<T, TMotor, TEncoder>{
protected:
  float nowTargetSpeed = 0;
  float accel;
  int16_t maxSpeed;
  T Kp, lowKp;
  int32_t lowGainArea;

  int32_t targetPosition;

  bool stopFlag;

public:
  MotorPositionController(T _Kp,T _Ki,T _Kd,T _lowKp,T isat=0, float _accel=0,int16_t _maxSpeed=0, int32_t _lowGainArea=0): MotorController<T, TMotor, TEncoder>(_Kp,_Ki,_Kd,isat){
    accel = abs(_accel);
    maxSpeed = abs(_maxSpeed);
    TEncoder::setPosition(0);
    Kp = _Kp;
    lowKp = _lowKp;
    lowGainArea = abs(_lowGainArea);

    targetPosition = 0;
    stopFlag = false;
  }

  int16_t getNowTargetSpeed() const{
    return nowTargetSpeed;
  }

  void setTargetPosition(int32_t l){
    targetPosition = l;
  }

  void update(){

      int32_t l = targetPosition;

      // NOTE: should be called after captureSpeed
      int16_t x=TEncoder::getSpeed();
      int32_t nowPosition=TEncoder::getPosition();
      int32_t target = l-nowPosition;

      int CoundToStop = ( abs(nowTargetSpeed) +accel-1 )/accel;
      int MoveToStop = nowTargetSpeed * CoundToStop /2;

      if(lowGainArea > abs(target)){
  	MotorController<T, TMotor, TEncoder>::pid.setKp(lowKp);
      }
      else{
  	MotorController<T, TMotor, TEncoder>::pid.setKp(Kp);
      }

      if( (target>=0 && target > MoveToStop) || ( target<0 && target < MoveToStop ) ){//accelerate

  	if( target>=0 && accel > target){
  	    nowTargetSpeed = target/2;
  	}

  	if( target<0 && -accel < target){
  	    nowTargetSpeed = target/2;
  	}

  	if( target > 0 ){
  	    nowTargetSpeed += accel;
  	    if( nowTargetSpeed > maxSpeed ) nowTargetSpeed = maxSpeed;
  	}
  	else{
  	    nowTargetSpeed -= accel;
  	    if( nowTargetSpeed < -maxSpeed ) nowTargetSpeed = -maxSpeed;
  	}
      }
      else{//decelerate
  	if( target > 0 ){
  	    nowTargetSpeed -= accel;
  	    if( nowTargetSpeed < 0 ) nowTargetSpeed = 0;
  	}
  	else{
  	    nowTargetSpeed += accel;
  	    if( nowTargetSpeed > 0 ) nowTargetSpeed = 0;
  	}
      }
      if( !stopFlag ){
	  MotorController<T, TMotor, TEncoder>::pid.update( (T)nowTargetSpeed ,x);
	  TMotor::setOutput(MotorController<T, TMotor, TEncoder>::pid.getOutput());
      }
      else{
	  TMotor::setOutput(0);
      }
    }

  void update(int32_t l){
    setTargetPosition(l);
    update();
  }

  //Stop motor until reset
  void emergencyStop(){
    TMotor::stop();
    stopFlag = true;
  }

};

