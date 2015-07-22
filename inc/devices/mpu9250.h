#include "config/stm32plus.h"
#include "utils/debug.h"

#include "config/gpio.h"
#include "config/spi.h"

using namespace stm32plus;

class MPU9250{
protected:
  Spi& spi;
public:
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
	MPU9250(Spi& _spi): spi(_spi){
		spi.setNss(true);
	}
	bool test(){
		uint8_t rx;
		readByte((uint8_t)0x75, rx);
		return (rx==0x71);
	}
	void setup(){
		writeByte(0x19, 0x00);
		//writeByte(0x24, {0b01011101});
		//writeByte(0x25, {0x80|0x1e});
		writeByte(0x6b, 0x00);
		setGyrRange(GYR_RANGE_2000DPS);
		setAccRange(ACC_RANGE_8G);
		writeByte(0x37, 0x02);	// Enable I2C Bypass mode
	}
	inline int16_t readInt16(uint8_t addr){
		union{
			uint16_t u;
			int16_t i;
		} _u2i;
		unsigned char rx[2];
		readBytes(addr,rx,2);
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
	  readBytes(0x3b,buf,6);
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
	  readBytes(0x43,buf,6);
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
		readBytes(0x3b,buf,14);
	}
	inline void setGyrRange(uint8_t range){
		writeByte(0x1b,(range&3)<<3);
	}
	inline void setAccRange(uint8_t range){
		writeByte(0x1c,(range&3)<<3);
	}

	void writeByte( uint8_t WriteAddr, uint8_t WriteData )
	{
		uint8_t data[2] = { WriteAddr , WriteData };
		spi.setNss(false);
		while(!spi.readyToSend());
		spi.send(data,2);
		spi.setNss(true);
	}

	void readByte(uint8_t addr, uint8_t &rx){
		addr |= 0x80;
		spi.setNss(false);
		spi.send(&addr,1);
		spi.receive(&rx,1);
		spi.setNss(true);
	}

	void readBytes(uint8_t addr, uint8_t *rx, int bytes){
		uint8_t send[bytes] = { 0x00 };
		send[0] = 0x80 | addr;

		spi.setNss(false);
		spi.send(send,1);
		spi.send(send+1,bytes-1,rx);
		spi.setNss(true);
	}

private:
};

