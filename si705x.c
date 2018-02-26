#include "si705x.h"

static struct i2c_m_sync_desc si705x_sync;

/* read a single register */
static uint8_t readReg(const uint8_t REG)
{
    uint8_t retval;
    i2c_m_sync_cmd_read(&si705x_sync, REG, &retval, 1);
    return retval;
}

/* write a single register */
static void writeReg(const uint8_t REG, uint8_t val)
{
    i2c_m_sync_cmd_write(&si705x_sync, REG, &val, 1);
}

bool si705x_init(struct i2c_m_sync_desc* const WIRE_I2C)
{
	si705x_sync = *WIRE_I2C;
	i2c_m_sync_enable(&si705x_sync);
	i2c_m_sync_set_slaveaddr(&si705x_sync, TEMP_ADDR, I2C_M_SEVEN);
	return true;
}

bool si705x_set_resolution(const SI705X_RESOLUTIONS_t RES)
{
	writeReg(TEMP_WRITE_USER1, RES);
	return true;	
}

uint16_t si705x_measure() 
{
	struct _i2c_m_msg msg;
	int32_t			err;
	uint16_t        ret = -1;
	uint8_t			Reg = TEMP_MEASURE_NOHOLD;
	msg.addr   = si705x_sync.slave_addr;
	msg.len    = 1;
	msg.flags  = 0;
	msg.buffer = &Reg;

	err = _i2c_m_sync_transfer(&si705x_sync.device, &msg);

	if (err != 0) {
		/* error occurred */
		return ret;
	}

	msg.flags  = I2C_M_STOP | I2C_M_RD;
	msg.buffer = (uint8_t*)&ret;
	msg.len    = 2;
	do{
		err = _i2c_m_sync_transfer(&si705x_sync.device, &msg);
	
	} while (err != 0);

	return ret;
}

bool si705x_voltage_ok()
{
	bool retval = false;
	
	if (readReg(TEMP_READ_USER1) & TEMP_VDD_LOW){
		retval = false;
	}
	else{
		retval = true;
	}
	
	return retval;
}

uint8_t si705x_fw_version()
{
	if (readReg(TEMP_READ_FW_VER1) == TEMP_FW1){
		return TEMP_FW1;
	}else if (readReg(TEMP_READ_FW_VER2) == TEMP_FW2){
		return TEMP_FW2;
	}
}

void si705x_reset()
{
	writeReg(TEMP_WRITE_USER1, TEMP_RESET);
}
