/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
#include "core/arterytek/at32f415/analog/input/CoreAnalogInputPin.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using core::arterytek::at32f415::analog::input::CoreAnalogInputPin;
using core::arterytek::at32f415::analog::input::CoreAnalogInputPort;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */
 
/**
 * @brief Construct a new Core Analog Input Pin object
 * 
 */
CoreAnalogInputPin::CoreAnalogInputPin(CoreAnalogInputPort& coreAnalogInputPort, uint16_t channel) construct 
  mCoreAnalogInputPort(coreAnalogInputPort){
    
  this->mChannel = channel;
}
/**
 * @brief Destroy the Core Analog Input Pin object
 * 
 */
CoreAnalogInputPin::~CoreAnalogInputPin(void){
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override>
 */

/**
 * @brief 
 * 
 * @return uint32_t 
 */

uint32_t CoreAnalogInputPin::convert(void){
  return this->mCoreAnalogInputPort.read(this->mChannel);
}

    /**
     * @brief Get the adc convert bit.
     * 
     * @return uint32_t 
     */
uint32_t CoreAnalogInputPin::getConvertBit(void){
  return this->mCoreAnalogInputPort.getConvertBit();
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
