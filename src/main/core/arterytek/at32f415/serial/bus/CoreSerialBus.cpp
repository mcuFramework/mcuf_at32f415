/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */

#include "core/arterytek/at32f415/serial/bus/CoreSerialBus.h"

//-----------------------------------------------------------------------------------------
#include "bsp_arterytek_at32f415.h"
#include "mcuf.h"

//-----------------------------------------------------------------------------------------
#include "core/arterytek/at32f415/Core.h"
#include "core/arterytek/at32f415/CoreInterrupt.h"

/* ****************************************************************************************
 * Macro
 */
#define REGNUMB                  (static_cast<char>(this->mRegister))
#define CONFIG                   (CoreSerialBus::mConfig[REGNUMB])
#define BASE                     ((i2c_type*)CONFIG.mBase)

/* ****************************************************************************************
 * Using
 */

using core::arterytek::at32f415::serial::bus::CoreSerialBus;

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using core::arterytek::at32f415::serial::bus::CoreSerialBusReg;
using core::arterytek::at32f415::serial::bus::CoreSerialBusConfig;
using mcuf::io::ByteBuffer;
using mcuf::hal::serial::bus::SerialBusEvent;

/* ****************************************************************************************
 * Variable <Static>
 */
const CoreSerialBusConfig CoreSerialBus::mConfig[2] = {
  {I2C1 , CRM_I2C1_PERIPH_CLOCK , CoreInterrupt::IRQ_I2C1_EVT,  CoreInterrupt::IRQ_I2C1_ERR},
  {I2C2 , CRM_I2C2_PERIPH_CLOCK , CoreInterrupt::IRQ_I2C2_EVT,  CoreInterrupt::IRQ_I2C2_ERR},
};

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new Core Serial Bus object
 * 
 * @param reg 
 */
CoreSerialBus::CoreSerialBus(CoreSerialBusReg reg) construct 
  mCoreSerialBusErrorEvent(*this){
  this->mRegister = reg;
  this->mByteBuffer = nullptr;
}

/**
 * @brief Destroy the Core Serial Bus object
 * 
 */
CoreSerialBus::~CoreSerialBus(void){
  this->deinit();
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mcuf::hal::InterruptEvent
 */

/**
 * @brief 
 * 
 */
void CoreSerialBus::interruptEvent(void){

}   

/* ****************************************************************************************
 * Public Method <Override> - mcuf::hal::Base
 */

/**
 * @brief uninitialze hardware.
 * 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::deinit(void){
  if(!this->init())
    return false;
  
  Core::interrupt.irqEnable(CONFIG.mIrqEvent, false);
  Core::interrupt.irqEnable(CONFIG.mIrqError, false);
  Core::interrupt.setHandler(CONFIG.mIrqEvent, nullptr);
  Core::interrupt.setHandler(CONFIG.mIrqError, nullptr);
  
  crm_periph_clock_enable(static_cast<crm_periph_clock_type>(CONFIG.mCmr), FALSE);
  
  return true;
}

/**
 * @brief initialze hardware;
 * 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::init(void){
  if(this->init())
    return false;
  
  crm_periph_clock_enable(static_cast<crm_periph_clock_type>(CONFIG.mCmr), TRUE);
  Core::interrupt.setHandler(CONFIG.mIrqEvent, this);
  Core::interrupt.setHandler(CONFIG.mIrqError, &this->mCoreSerialBusErrorEvent);
  
  Core::interrupt.irqEnable(CONFIG.mIrqEvent, true);
  Core::interrupt.irqEnable(CONFIG.mIrqError, true);
  
  i2c_init(BASE, I2C_FSMODE_DUTY_2_1, 100000);
  i2c_own_address1_set(BASE, I2C_ADDRESS_MODE_7BIT, 0x00);
  
  return true;
}

/**
 * @brief get hardware initialzed status.
 * 
 * @return true not init
 * @return false initd
 */
bool CoreSerialBus::isInit(void){
  return CRM_REG(CONFIG.mCmr) & CRM_REG_BIT(CONFIG.mCmr);
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::hal::serial::bus::SerialBusControl
 */

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t CoreSerialBus::clockRate(void){
  if(!this->isInit())
    return 0;
  
  uint32_t result = 0;
  uint32_t i2c_clock = BASE->ctrl2_bit.clkfreq * 500000;
  result = i2c_clock /= BASE->clkctrl_bit.speed;
  return result;
}

/**
 * @brief 
 * 
 * @param clock 
 * @return uint32_t 
 */
uint32_t CoreSerialBus::clockRate(uint32_t clock){
  if(!this->isInit())
    return 0;
  
  if(this->isBusy())
    return 0;
  
  BASE->ctrl1_bit.i2cen = false;
  uint32_t i2c_clock = BASE->ctrl2_bit.clkfreq * 1000000;
  BASE->clkctrl_bit.speed = (uint16_t)(i2c_clock / (clock << 1));
  BASE->ctrl1_bit.i2cen = true;
  return this->clockRate();
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::hal::serial::bus::SerialBusTransfer
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::abort(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::isBusy(void){
  if(this->mByteBuffer != nullptr)
    return true;
  
  return false;
}

/**
 * @brief 
 * 
 * @param address 
 * @param receiver 
 * @param event 
 */
bool CoreSerialBus::read(uint8_t address, ByteBuffer& receiver, SerialBusEvent* event){
  if(this->isBusy())
    return false;
  
  this->mByteBuffer = &receiver;
  this->mEvent = event;
  
  
  return false;
}
/**
 * @brief 
 * 
 * @param address 
 * @param receiver 
 * @param event 
 */
bool CoreSerialBus::write(uint16_t address, ByteBuffer& receiver, SerialBusEvent* event){
  if(this->isBusy())
    return false;
  
  return false;
}

/**
 * @brief 
 * 
 * @param address 
 * @param transfer 
 * @param receiver 
 * @param event 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::writeAfterRead(uint16_t address, ByteBuffer& transfer, ByteBuffer& receiver, SerialBusEvent* event){
  if(this->isBusy())
    return false;
  
  return false;
}

/* ****************************************************************************************
 * Public Method
 */

/* ****************************************************************************************
 * Protected Method <Static>
 */

/* ****************************************************************************************
 * Protected Method <Override>
 */

/* ****************************************************************************************
 * Protected Method
 */

/* ****************************************************************************************
 * Private Method
 */

/* ****************************************************************************************
 * End of file
 */
