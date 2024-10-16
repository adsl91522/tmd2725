#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include <tmd2725.h>

enum {
    TMD2725_OK = 0,
    TMD2725_ERROR = -1,
};

#define I2C_ADDR                        0x39

static const struct device *tmd2725_i2c = NULL;

LOG_MODULE_REGISTER(tmd2725, LOG_LEVEL_WRN);

int tmd2725_enable(uint8_t en_flag)
{ 
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, ENABLE, en_flag) != 0){
        LOG_WRN("I2c tmd2725_enable write fail\r\n");
        return TMD2725_ERROR;
    }
    return TMD2725_OK;
}

int tmd2725_atime(uint8_t value)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, ATIME, value) != 0){
        LOG_WRN("I2c tmd2725_atime write fail\r\n");
        return TMD2725_ERROR;
    }
    return TMD2725_OK;
}

int tmd2725_prate(uint8_t value)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PRATE, value) != 0){
        LOG_WRN("I2c tmd2725_prate write fail\r\n");
        return TMD2725_ERROR;
    }
    return TMD2725_OK;
}

int tmd2725_prox_threshold(uint8_t piht, uint8_t pilt)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PILT, pilt) != 0){
        LOG_WRN("I2c tmd2725_prox_threshold write fail\r\n");
        return TMD2725_ERROR;
    }

    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PIHT, piht) != 0){
        LOG_WRN("I2c tmd2725_prox_threshold write fail\r\n");
        return TMD2725_ERROR;
    }


    return TMD2725_OK;
}

int tmd2725_pers(uint8_t ppers, uint8_t apers)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PERS, ((ppers<<4)+apers)&0xFF) != 0){
        LOG_WRN("I2c tmd2725_pers write fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_pcfg0(uint8_t pgain1, uint8_t ppulse)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PCFG0, (pgain1+(ppulse-1))&0xFF) != 0){
        LOG_WRN("I2c tmd2725_pcfg0 write fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_pcfg1(uint8_t pgain, uint8_t pldrive)
{ 
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, PCFG1, (pgain+pldrive)&0xFF) != 0){
        LOG_WRN("I2c tmd2725_pcfg1 write fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;   
}

int tmd2725_cfg1(uint8_t again)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, CFG1, again) != 0){
        LOG_WRN("I2c tmd2725_cfg1 write fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_status_reg(uint8_t *buf)
{
    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, STATUS, buf) != 0){
        LOG_WRN("I2c tmd2725_status_reg read fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_photopic(uint16_t *photopic_data)
{
    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, PHOTOPICH, (uint8_t *)photopic_data) != 0){
        LOG_WRN("I2c tmd2725_photopic read fail\r\n");
        return TMD2725_ERROR;
    }

    *photopic_data<<=8; 

    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, PHOTOPICL, (uint8_t *)photopic_data) != 0){
        LOG_WRN("I2c tmd2725_photopic read fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_als_ir(uint16_t *ir_data)
{
    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, ALS_IRH, (uint8_t *)ir_data) != 0){
        LOG_WRN("I2c tmd2725_als_ir read fail\r\n");
        return TMD2725_ERROR;
    }

    *ir_data<<=8; 

    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, ALS_IRL, (uint8_t *)ir_data) != 0){
        LOG_WRN("I2c tmd2725_als_ir read fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_pdata(uint8_t *pdata)
{
    if(i2c_reg_read_byte(tmd2725_i2c, I2C_ADDR, PDATA, pdata) != 0){
        LOG_WRN("I2c tmd2725_pdata read fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_cfg3(uint8_t cfg3_flag)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, CFG3, cfg3_flag) != 0){
        LOG_WRN("I2c tmd2725_cfg1 write fail\r\n");
        return TMD2725_ERROR;
    }

    return TMD2725_OK;
}

int tmd2725_poffset(int poffset)
{
    if(poffset < 0){
        poffset+=255;
    }else{
        poffset+=256;
    }
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, POFFSET_H, (poffset>>8)&0xFF) != 0){
        LOG_WRN("I2c tmd2725_poffset write fail\r\n");
        return TMD2725_ERROR;
    }

    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, POFFSET_L, poffset&0xFF) != 0){
        LOG_WRN("I2c tmd2725_poffset write fail\r\n");
        return TMD2725_ERROR;
    }
    return TMD2725_OK;
}

int tmd2725_intenab(uint8_t isr_flag)
{
    if(i2c_reg_write_byte(tmd2725_i2c, I2C_ADDR, INTENAB, isr_flag) != 0){
        LOG_WRN("I2c tmd2725_intenab write fail\r\n");
        return TMD2725_ERROR;
    }
    return TMD2725_OK;
}

void tmd2725_i2c_init(const struct device *i2c)
{
    tmd2725_i2c = i2c;
}
