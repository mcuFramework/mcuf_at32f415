/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef CORE_ARTERYTEK_AT32F415_62A83E55_7F35_4500_9AD9_5ED9126A04AE
#define CORE_ARTERYTEK_AT32F415_62A83E55_7F35_4500_9AD9_5ED9126A04AE

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"
#include "core_arterytek_at32f415.h"

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Namespace
 */  
namespace tool{
  class BoardPeriph;
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class tool::BoardPeriph extends mcuf::lang::Object{

  /* **************************************************************************************
   * Variable <Public>
   */
  public:
    core::arterytek::at32f415::general::pin::CoreGeneralPin led[8];
    core::arterytek::at32f415::general::pin::CoreGeneralPin wakeup;
    core::arterytek::at32f415::general::pin::CoreGeneralPin function;

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */

  /* **************************************************************************************
   * Abstract method <Public>
   */

  /* **************************************************************************************
   * Abstract method <Protected>
   */

  /* **************************************************************************************
   * Construct Method
   */
  public: 
    
    /**
     * @brief Construct a new Board Periph object
     * 
     */
    BoardPeriph(void);

    /**
     * @brief Destroy the Board Periph object
     * 
     */
    virtual ~BoardPeriph(void) = default;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override>
   */

  /* **************************************************************************************
   * Public Method
   */

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

/* ****************************************************************************************
 * End of file
 */ 

#endif /* CORE_ARTERYTEK_AT32F415_62A83E55_7F35_4500_9AD9_5ED9126A04AE */
