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

static uint8_t bitswap(uint8_t input){
    unsigned char output = 0;
    for (int k = 0; k < 8; k++) {
        output |= ((input >> k) & 0x01) << (7 - k);
    }
    return output;
}

static uint8_t crc8(void* inData, uint8_t len, uint8_t init){
    uint8_t* data = (uint8_t*)inData;
    uint8_t crc = bitswap(init);

    for (int i=0; i<len;i++)
    {
        uint8_t inbyte = bitswap(data[i]);
        for (uint8_t j=0;j<8;j++)
        {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix)
            crc ^= 0x8C;

            inbyte >>= 1;
        }
    }
    return bitswap(crc);
}


uint16_t si705x_measure(int32_t* error)
{
	struct _i2c_m_msg msg;
	int32_t			err;
    uint8_t         buf[3];
	uint8_t			Reg    = TEMP_MEASURE_NOHOLD;
	
    msg.addr   = si705x_sync.slave_addr;
	msg.len    = 1;
	msg.flags  = 0;
	msg.buffer = &Reg;

	err = _i2c_m_sync_transfer(&si705x_sync.device, &msg);

	if (err != 0) {
		/* error occurred */
        if(error != NULL) {
		    *error = -1;
        }
        return 0;
	}

	msg.flags  = I2C_M_STOP | I2C_M_RD;
	msg.buffer = buf;
	msg.len    = 3;     // two for temp, 3 for temp plus checksum
	do{
		err = _i2c_m_sync_transfer(&si705x_sync.device, &msg);
	} while (err != 0);

    if(error != NULL) {
        if(crc8(buf, 2, 0x00) == buf[2]) {
            *error = 0;
        }
        else {
            *error = -2;
        }   
    }

	return (buf[0] << 8) | buf[1];
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
    uint8_t fwVer = 0;
	if (readReg(TEMP_READ_FW_VER1) == TEMP_FW1){
		fwVer = TEMP_FW1;
	}else if (readReg(TEMP_READ_FW_VER2) == TEMP_FW2){
		fwVer = TEMP_FW2;
	}
    return fwVer;
}

void si705x_reset()
{
	writeReg(TEMP_WRITE_USER1, TEMP_RESET);
}
