#include "si705x.h"

static struct i2c_m_sync_desc si705x_sync;

/* read a single register */
static uint8_t readReg(const uint8_t REG)
{
    uint8_t retval;
    i2c_m_sync_cmd_read(&max44009_sync, REG, &retval, 1);
    return retval;
}

/* write a single register */
static void writeReg(const uint8_t REG, uint8_t val)
{
    i2c_m_sync_cmd_write(&max44009_sync, REG, &val, 1);
}

bool si705x_init(struct i2c_m_async_desc* const WIRE_I2C)
{

}

bool si705x_set_resolution(const uint8_t RES)
{

}

uint16_t si705x_measure(const bool MASTER_HOLD)
{

}

bool si705x_voltage_ok()
{

}

uint32_t si705x_fw_version()
{

}

float si705x_reset()
{

}
