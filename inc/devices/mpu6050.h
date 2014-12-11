#include "config/stm32plus.h"
#include "config/i2c.h"
#include "config/timing.h"
#include "config/usart.h"
#include "config/stream.h"

using namespace stm32plus;

template<class TI2C,int TAD0>
class MPU6050 : public TI2C{
public:
	enum {
		SLAVE_ADDRESS = (0x68|(!!TAD0))<<1
	};
	enum{
		GYR_RANGE_250DPS=0,
		GYR_RANGE_500DPS=1,
		GYR_RANGE_1000DPS=2,
		GYR_RANGE_2000DPS=3
	};
	enum{
		ACC_RANGE_2G=0,
		ACC_RANGE_4G=1,
		ACC_RANGE_8G=2,
		ACC_RANGE_16G=3
	};
	MPU6050(typename I2C::Parameters params):TI2C(params){
		this->setSlaveAddress(SLAVE_ADDRESS);
		this->enablePeripheral();
		MillisecondTimer::initialise();
	}
	bool test(){
		unsigned char rx;
		TI2C::readByte(0x75, rx);
		return (rx==0x68);
	}
	void setup(){
		TI2C::writeByte(0x19, 0x00);
		//TI2C::writeByte(0x24, {0b01011101});
		//TI2C::writeByte(0x25, {0x80|0x1e});
		TI2C::writeByte(0x6b, 0x00);
		setGyrRange(GYR_RANGE_2000DPS);
		setAccRange(ACC_RANGE_8G);
		TI2C::writeByte(0x37, 0x02);	// Enable I2C Bypass mode
	}
	inline int16_t readInt16(uint8_t addr){
		union{
			uint16_t u;
			int16_t i;
		} _u2i;
		unsigned char rx[2];
		TI2C::readBytes(addr,rx,2);
		_u2i.u=(rx[0]<<8)|rx[1];
		return _u2i.i;
	}
	inline int16_t readAccX(){
		return readInt16(0x3b);
	}
	inline int16_t readAccY(){
		return readInt16(0x3d);
	}
	inline int16_t readAccZ(){
		return readInt16(0x3f);
	}
	inline int16_t readTemp(){
		return readInt16(0x41);
	}
	inline int16_t readGyrX(){
		return readInt16(0x43);
	}
	inline int16_t readGyrY(){
		return readInt16(0x45);
	}
	inline int16_t readGyrZ(){
		return readInt16(0x47);
	}
	inline void readAccAll(int16_t *acc){
	  uint8_t buf[6];
	  TI2C::readBytes(0x3b,buf,6);
	  union{
	    uint16_t u;
	    int16_t i;
	  } _u2i;
	  _u2i.u=(buf[0]<<8)|buf[1];
	  acc[0] = _u2i.i;
	  _u2i.u=(buf[2]<<8)|buf[3];
	  acc[1] = _u2i.i;
	  _u2i.u=(buf[4]<<8)|buf[5];
	  acc[2] = _u2i.i;
	}
	inline void readGyrAll(int16_t *gyr){
	  uint8_t buf[6];
	  TI2C::readBytes(0x43,buf,6);
	  union{
	    uint16_t u;
	    int16_t i;
	  } _u2i;
	  _u2i.u=(buf[0]<<8)|buf[1];
	  gyr[0] = _u2i.i;
	  _u2i.u=(buf[2]<<8)|buf[3];
	  gyr[1] = _u2i.i;
	  _u2i.u=(buf[4]<<8)|buf[5];
	  gyr[2] = _u2i.i;
	}
	inline void readAll(uint8_t *buf){
		TI2C::readBytes(0x3b,buf,14);
	}
	inline void setGyrRange(uint8_t range){
		TI2C::writeByte(0x1b,(range&3)<<3);
	}
	inline void setAccRange(uint8_t range){
		TI2C::writeByte(0x1c,(range&3)<<3);
	}
private:
};

