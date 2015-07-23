#include "config/stm32plus.h"
#include "utils/debug.h"

#include "config/timing.h"
#include "config/spi.h"

using namespace stm32plus;

#define AK8963_I2C_ADDR 0x0C
#define MPU9250_READ_FLAG   0x80

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

		writeByte(PWR_MGMT_1, 0x80);
		MillisecondTimer::delay(100);
		writeByte(PWR_MGMT_1, 0x00);

		setGyrRange(GYR_RANGE_2000DPS);
		setAccRange(ACC_RANGE_8G);

		writeByte(USER_CTRL, 0x34); // Enable Master I2C, disable primary I2C I/F, and reset FIFO.
		writeByte(SMPLRT_DIV, 5); // SMPLRT_DIV = 9, 100Hz sampling;
		writeByte(CONFIG, (1 << 6) | (1 << 0)); // FIFO_mode = 1 (accept overflow), Use LPF, Bandwidth_gyro = 184 Hz, Bandwidth_temperature = 188 Hz,
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
		spi.send(data,2);
		spi.setNss(true);
		for(volatile int i=0;i<30;i++);
	}

	void readByte(uint8_t addr, uint8_t &rx){
		addr |= MPU9250_READ_FLAG;
		spi.setNss(false);
		spi.send(&addr,1);
		spi.receive(&rx,1);
		spi.setNss(true);
		for(volatile int i=0;i<30;i++);
	}

	void readBytes(uint8_t addr, uint8_t *rx, int bytes){
		addr |= MPU9250_READ_FLAG;
		spi.setNss(false);
		spi.send(&addr,1);
		spi.receive(rx,bytes-1);
		spi.setNss(true);
		for(volatile int i=0;i<30;i++);
	}

private:
	enum {
	  SELF_TEST_X_GYRO = 0x00,
	  SELF_TEST_Y_GYRO = 0x01,
	  SELF_TEST_Z_GYRO = 0x02,
	  SELF_TEST_X_ACCEL = 0x0D,
	  SELF_TEST_Y_ACCEL = 0x0E,
	  SELF_TEST_Z_ACCEL = 0x0F,
	  XG_OFFSET_H = 0x13,
	  XG_OFFSET_L = 0x14,
	  YG_OFFSET_H = 0x15,
	  YG_OFFSET_L = 0x16,
	  ZG_OFFSET_H = 0x17,
	  ZG_OFFSET_L = 0x18,
	  SMPLRT_DIV = 0x19,
	  CONFIG = 0x1A,
	  GYRO_CONFIG = 0x1B,
	  ACCEL_CONFIG = 0x1C,
	  ACCEL_CONFIG2 = 0x1D,
	  LP_ACCEL_ODR = 0x1E,
	  WOM_THR = 0x1F,
	  FIFO_EN = 0x23,
	  I2C_MST_CTRL = 0x24,
	  I2C_SLV0_ADDR = 0x25,
	  I2C_SLV0_REG = 0x26,
	  I2C_SLV0_CTRL = 0x27,
	  I2C_SLV1_ADDR = 0x28,
	  I2C_SLV1_REG = 0x29,
	  I2C_SLV1_CTRL = 0x2A,
	  I2C_SLV2_ADDR = 0x2B,
	  I2C_SLV2_REG = 0x2C,
	  I2C_SLV2_CTRL = 0x2D,
	  I2C_SLV3_ADDR = 0x2E,
	  I2C_SLV3_REG = 0x2F,
	  I2C_SLV3_CTRL = 0x30,
	  I2C_SLV4_ADDR = 0x31,
	  I2C_SLV4_REG = 0x32,
	  I2C_SLV4_DO = 0x33,
	  I2C_SLV4_CTRL = 0x34,
	  I2C_SLV4_DI = 0x35,
	  I2C_MST_STATUS = 0x36,
	  INT_PIN_CFG = 0x37,
	  INT_ENABLE = 0x38,
	  INT_STATUS = 0x3A,
	  ACCEL_OUT_BASE = 0x3B,
	  TEMP_OUT_BASE = 0x41,
	  GYRO_OUT_BASE = 0x43,
	  EXT_SENS_DATA_BASE = 0x49,
	  I2C_SLV0_DO = 0x63,
	  I2C_SLV1_DO = 0x64,
	  I2C_SLV2_DO = 0x65,
	  I2C_SLV3_DO = 0x66,
	  I2C_MST_DELAY_CTRL = 0x67,
	  SIGNAL_PATH_RESET = 0x68,
	  MOT_DETECT_CTRL = 0x69,
	  USER_CTRL = 0x6A,
	  PWR_MGMT_1 = 0x6B,
	  PWR_MGMT_2 = 0x6C,
	  FIFO_COUNTH = 0x72,
	  FIFO_COUNTL = 0x73,
	  FIFO_R_W = 0x74,
	  WHO_AM_I = 0x75,
	  XA_OFFSET_H = 0x77,
	  XA_OFFSET_L = 0x78,
	  YA_OFFSET_H = 0x7A,
	  YA_OFFSET_L = 0x7B,
	  ZA_OFFSET_H = 0x7D,
	  ZA_OFFSET_L = 0x7E,
	};
	enum {
	  AK8963_WIA    = 0x00, // Device ID
	  AK8963_INFO   = 0x01, // Information
	  AK8963_ST1    = 0x02, // Status 1
	  AK8963_HXL    = 0x03, // Measurement data
	  AK8963_HXH    = 0x04,
	  AK8963_HYL    = 0x05,
	  AK8963_HYH    = 0x06,
	  AK8963_HZL    = 0x07,
	  AK8963_HZH    = 0x08,
	  AK8963_ST2    = 0x09, // Status 2
	  AK8963_CNTL1  = 0x0A, // Control 1
	  AK8963_CNTL2  = 0x0B, // Control 2
	  AK8963_RSV    = 0x0B, // Reserved
	  AK8963_ASTC   = 0x0C, // Self-test
	  AK8963_TS1    = 0x0D, // Test 1
	  AK8963_TS2    = 0x0E, // Test 2
	  AK8963_I2CDIS = 0x0F, // I2C disable
	  AK8963_ASAX   = 0x10, // X-axis sensitivity adjustment value
	  AK8963_ASAY   = 0x11, // Y-axis sensitivity adjustment value
	  AK8963_ASAZ   = 0x12, // Z-axis sensitivity adjustment value
	};
};

