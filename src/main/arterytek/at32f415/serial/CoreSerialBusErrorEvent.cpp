/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */
 
#include "arterytek/at32f415/serial/CoreSerialBusErrorEvent.h"

//-----------------------------------------------------------------------------------------
#include "bsp_arterytek_at32f415.h"
#include "mcuf.h"

//-----------------------------------------------------------------------------------------
#include "arterytek/at32f415/serial/CoreSerialBus.h"

/* ****************************************************************************************
 * Macro
 */
#define REGNUMB                  (static_cast<unsigned char>(this->mBase.mRegister))
#define CONFIG                   (CoreSerialBus::mConfig[REGNUMB])
#define BASE                     ((i2c_type*)CONFIG.mBase)

/* ****************************************************************************************
 * Using
 */

using arterytek::at32f415::serial::CoreSerialBusErrorEvent;

//-----------------------------------------------------------------------------------------
using namespace hal::serial;

//-----------------------------------------------------------------------------------------
using arterytek::at32f415::serial::CoreSerialBus;
using mcuf::io::ByteBuffer;
using mcuf::lang::System;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new Core Serial Bus Error Event object
 * 
 * @param base 
 */
CoreSerialBusErrorEvent::CoreSerialBusErrorEvent(CoreSerialBus& base) : 
mBase(base){
  return;
}
  
/**
 *
 */
CoreSerialBusErrorEvent::~CoreSerialBusErrorEvent(void){
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - hal::InterruptEvent
 */

/**
 * @brief 
 * 
 */
void CoreSerialBusErrorEvent::interruptEvent(void){
  i2c_type* base = BASE;
  
  base->ctrl1_bit.acken = false;
  base->ctrl1_bit.genstop = true;
  
  i2c_interrupt_enable(base, I2C_EVT_INT | I2C_DATA_INT | I2C_ERR_INT, FALSE); 
  this->mBase.statusClear();
  base->sts1 = 0;
  
  System::execute(this->mBase);
  return;
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
