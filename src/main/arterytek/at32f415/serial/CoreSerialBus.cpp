/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */

#include "arterytek/at32f415/serial/CoreSerialBus.h"

//-----------------------------------------------------------------------------------------
#include "bsp_arterytek_at32f415.h"
#include "mcuf.h"

//-----------------------------------------------------------------------------------------
#include "arterytek/at32f415/Core.h"
#include "arterytek/at32f415/CoreInterrupt.h"

/* ****************************************************************************************
 * Macro
 */
#define REGNUMB                  (static_cast<unsigned char>(this->mRegister))
#define CONFIG                   (CoreSerialBus::mConfig[REGNUMB])
#define BASE                     ((i2c_type*)CONFIG.mBase)

/* ****************************************************************************************
 * Using
 */

using arterytek::at32f415::serial::CoreSerialBus;

//-----------------------------------------------------------------------------------------
using namespace hal::serial;

//-----------------------------------------------------------------------------------------
using arterytek::at32f415::serial::CoreSerialBusReg;
using arterytek::at32f415::serial::CoreSerialBusConfig;
using mcuf::io::ByteBuffer;


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
CoreSerialBus::CoreSerialBus(CoreSerialBusReg reg) : 
  mCoreSerialBusErrorEvent(*this){
  this->mRegister = reg;
  this->mByteBuffer = nullptr;
  this->mByteBufferNext = nullptr;
  this->mDirect = 0;
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
 * Public Method <Override> - mcuf::function::Runnable
 */
 
/**
 *
 */
void CoreSerialBus::run(void){
  ByteBuffer* byteBuffer = this->mByteBuffer;
  SerialBusEvent* event = this->mEvent;
  SerialBusStatus status = this->mStatus;
  byteBuffer->position(byteBuffer->position() + this->mCount);
  void* attachment = this->mAttachment;
  int result = this->mLength - this->mCount;
  this->handlerClear();
  
  if(event)
    event->onSerialBusEvent(status, result ,attachment);
}  

/* ****************************************************************************************
 * Public Method <Override> - hal::InterruptEvent
 */

/**
 * @brief 
 * 
 */
void CoreSerialBus::interruptEvent(void){
  i2c_type* base = BASE;
  
  if(base->ctrl2_bit.dataien){
    if(base->sts1_bit.tdbe){ //write isr
      if(this->mCount >= this->mLength){ //write successful
        if(base->sts1_bit.tdc){
          this->mStatus = SerialBusStatus::WRITE_SUCCESSFUL;
          
          if(this->mByteBufferNext){
            this->afterRead();
            
          }else{
            base->ctrl1_bit.genstop = true;
            base->ctrl2 &= ~static_cast<uint32_t>(I2C_EVT_INT | I2C_DATA_INT | I2C_ERR_INT);
            mcuf::lang::System::execute(*this);
            
          }
        }

      }else{
        base->dt = this->mPointer[this->mCount++];
        
      }

    }else if(base->sts1_bit.rdbf){ //read isr
      uint8_t cache = (uint8_t)base->dt;
      if(this->mCount != 0)
        this->mPointer[this->mCount-1] = cache;
      
      ++this->mCount;
        
      if((this->mCount+1) == (this->mLength+1)){
        
        base->ctrl1_bit.acken = false;
        base->ctrl1_bit.genstop = true;
          
      }else if(this->mCount >= (this->mLength+1)){ //read successful
        
        this->mStatus = SerialBusStatus::READ_SUCCESSFUL;
        base->ctrl1_bit.genstop = true;
        base->ctrl2 &= ~static_cast<uint32_t>(I2C_EVT_INT | I2C_DATA_INT | I2C_ERR_INT);
        mcuf::lang::System::execute(*this);
        
      }
    }
    
  }
  
  if(base->ctrl2_bit.evtien){
    if(base->sts1_bit.startf){  //address
      if(this->mDirect)  //write
        base->dt = (this->mAddress & 0xFE);
    
      else  //read
        base->dt = (this->mAddress | 0x01);
    }else if(base->sts1_bit.addr7f){
      if(this->mDirect) //enable receiver
        base->ctrl1_bit.acken = false;
      
      else
        base->ctrl1_bit.acken = true;
      
      this->statusClear();
    
    }
  }
}   

/* ****************************************************************************************
 * Public Method <Override> - hal::Base
 */

/**
 * @brief uninitialze hardware.
 * 
 * @return true 
 * @return false 
 */
bool CoreSerialBus::deinit(void){
  if(!this->isInit())
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
  if(this->isInit())
    return false;
  
  crm_periph_clock_enable(static_cast<crm_periph_clock_type>(CONFIG.mCmr), TRUE);
  Core::interrupt.setHandler(CONFIG.mIrqEvent, this);
  Core::interrupt.setHandler(CONFIG.mIrqError, &this->mCoreSerialBusErrorEvent);
  
  Core::interrupt.irqEnable(CONFIG.mIrqEvent, true);
  Core::interrupt.irqEnable(CONFIG.mIrqError, true);
  
  i2c_init(BASE, I2C_FSMODE_DUTY_2_1, 100000);
  i2c_own_address1_set(BASE, I2C_ADDRESS_MODE_7BIT, 0x00);
  BASE->ctrl1_bit.i2cen = true;
  
  this->handlerClear();
  
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
 * Public Method <Override> - hal::serial::SerialBusControl
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
 * Public Method <Override> - hal::serial::SerialBusTransfer
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
bool CoreSerialBus::read(uint16_t address, ByteBuffer& receiver, void* attachment, SerialBusEvent* event){
  
  if(this->isBusy())
    return false;
  
  this->mDirect = 0;
  return this->handlerConfig(address, nullptr, &receiver, attachment, event);
}
/**
 * @brief 
 * 
 * @param address 
 * @param receiver 
 * @param event 
 */
bool CoreSerialBus::write(uint16_t address, ByteBuffer& transfer, void* attachment, SerialBusEvent* event){

  if(this->isBusy())
    return false;
  
  this->mDirect = 1;
  return this->handlerConfig(address, &transfer, nullptr, attachment, event);
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
bool CoreSerialBus::writeAfterRead(uint16_t address, ByteBuffer& transfer, ByteBuffer& receiver, void* attachment, SerialBusEvent* event){

  if(this->isBusy())
    return false;
  
  this->mDirect = 1;
  return this->handlerConfig(address, &transfer, &receiver, attachment, event);
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

bool CoreSerialBus::handlerConfig(uint16_t address, ByteBuffer* transfer, ByteBuffer* receiver, void* attachment, SerialBusEvent* event){
  if(receiver->isEmpty()) // check receiver has data in byeBuffer
    return false;
  
  if(transfer != nullptr){
    if(transfer->isEmpty()) // check transfer has data in byteBuffer
      return false;
    
    this->mByteBuffer = transfer;
    this->mByteBufferNext = receiver;
    
  }else{
    this->mByteBuffer = receiver;
    this->mByteBufferNext = nullptr;
    
  }
  
  this->mEvent = event;
  this->mAddress = static_cast<uint16_t>(address << 1);

  if(this->handlerFormat(*this->mByteBuffer)){
    if(this->begin())
      return true;
    
  }
  

  BASE->ctrl1_bit.acken = false;
  BASE->ctrl1_bit.genstop = true;
  
  if(this->mDirect)
    this->mStatus = SerialBusStatus::WRITE_FAIL;
  else
    this->mStatus = SerialBusStatus::READ_FAIL;
  
  this->mCoreSerialBusErrorEvent.run();
  return false;  
}

/**
 *
 */
bool CoreSerialBus::handlerFormat(ByteBuffer& byteBuffer){
  if(byteBuffer.isEmpty())
    return false;
  
  this->mLength = static_cast<uint16_t>(byteBuffer.remaining());
  this->mCount = 0;
  this->mPointer = static_cast<uint8_t*>(byteBuffer.pointer(byteBuffer.position()));
  
  return true;
}

/**
 *
 */
void CoreSerialBus::handlerClear(void){
  this->mByteBuffer = nullptr;
  this->mByteBufferNext = nullptr;
  this->mPointer = nullptr;
  this->mLength = 0;
  this->mCount = 0;
  this->mDirect = 0;
}

/**
 *
 */
bool CoreSerialBus::begin(void){
  i2c_interrupt_enable(BASE, I2C_EVT_INT | I2C_DATA_INT | I2C_ERR_INT, TRUE);
  BASE->ctrl1_bit.acken = false;
  BASE->ctrl1_bit.genstart = true;
  return true;
}

/**
 *
 */
void CoreSerialBus::statusClear(void){
  i2c_type* base = BASE;
  __IO uint32_t tmpreg;  
  tmpreg = base->sts1; 
  tmpreg = base->sts2;
}

/**
 *
 */
void CoreSerialBus::afterRead(void){
  this->mByteBuffer = this->mByteBufferNext;
  this->mByteBufferNext = nullptr;
  this->handlerFormat(*this->mByteBuffer);
  this->mDirect = 0;
  this->begin();
}

/* ****************************************************************************************
 * End of file
 */
