/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef CORE_ARTERYTEK_AT32F415_83A23821_E744_4D84_B7BA_21282E88B274
#define CORE_ARTERYTEK_AT32F415_83A23821_E744_4D84_B7BA_21282E88B274

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"
#include "core_arterytek_at32f415.h"

//-----------------------------------------------------------------------------------------
#include "tool/package-info.h"

/* ****************************************************************************************
 * Namespace
 */  
namespace core{
  namespace analog{
    namespace input{
      class CoreAnalogInputTest;
    }
  }
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class core::analog::input::CoreAnalogInputTest extends mcuf::lang::Object implements
  public mcuf::function::Runnable{

  /* **************************************************************************************
   * Variable <Public>
   */

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    mcuf::util::Stacker& mStacker;
    tool::Console* mConsole;
    
    core::arterytek::at32f415::analog::input::CoreAnalogInputPort* mCoreAnalogInputPort;

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
     *
     */
    CoreAnalogInputTest(mcuf::util::Stacker& stacker);

    /**
     *
     */
    virtual ~CoreAnalogInputTest(void);

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - mcuf::function::Runnable
   */
  public:
    
    /**
     * @brief 
     * 
     */
    virtual void run(void) override;

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
  private:
    
    /**
     *
     */
    void init(void);
  
    /**
     *
     */
    float getVoltage(uint16_t level);

};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* CORE_ARTERYTEK_AT32F415_83A23821_E744_4D84_B7BA_21282E88B274 */
