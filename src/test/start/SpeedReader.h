/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef CORE_ARTERYTEK_AT32F415_DB9DB91F_6D40_4397_BE38_B5C415EF5270
#define CORE_ARTERYTEK_AT32F415_DB9DB91F_6D40_4397_BE38_B5C415EF5270

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"
#include "core_arterytek_at32f415.h"

//-----------------------------------------------------------------------------------------
#include "periph/Test.hpp"

/* ****************************************************************************************
 * Namespace
 */  
namespace start{
  class SpeedReader;
}

/* ****************************************************************************************
 * Class Object
 */  
class start::SpeedReader extends mcuf::lang::Object
  implements mcuf::function::Runnable{

  /* **************************************************************************************
   * Subclass
   */
    
  /* **************************************************************************************
   * Variable <Public>
   */

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private: uint32_t mValue;
  private: uint32_t mCache;
  private: uint32_t mTotal;
  private: core::arterytek::at32f415::CoreEXTI mCoreEXTI;

  /* **************************************************************************************
   * Abstract method <Public>
   */

  /* **************************************************************************************
   * Abstract method <Protected>
   */

  /* **************************************************************************************
   * Construct Method
   */

  /**
   * Construct.
   */
  public: SpeedReader(core::arterytek::at32f415::CoreEXTI::Register reg);

  /**
   * Destruct.
   */
  public: ~SpeedReader(void);

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - mcuf::function::Runnable
   */
  
  /**
   *
   */
  public: virtual void run(void) override;
  
  /* **************************************************************************************
   * Public Method
   */
   
  /**
   *
   */
  public: void recode(void);
  
  /**
   *
   */
  public: uint32_t getValue(void);
  
  /**
   *
   */
  public: uint32_t getTotal(void);

  /* **************************************************************************************
   * Protected Method <Static>
   */

  /* **************************************************************************************
   * Protected Method <Override>
   */

  /* **************************************************************************************
   * Protected Method
   */

  /* **************************************************************************************
   * Private Method <Static>
   */

  /* **************************************************************************************
   * Private Method <Override>
   */
   
  /* **************************************************************************************
   * Private Method
   */  

};

/* *****************************************************************************************
 * End of file
 */ 

#endif/* CORE_ARTERYTEK_AT32F415_DB9DB91F_6D40_4397_BE38_B5C415EF5270 */
