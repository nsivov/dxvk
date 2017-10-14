#pragma once

#include "dxvk_include.h"

namespace dxvk {
  
  /**
   * \brief DXVK resource
   * 
   * Keeps track of whether the resource is currently in use
   * by the GPU. As soon as a command that uses the resource
   * is recorded, it will be marked as 'in use'.
   */
  class DxvkResource : public RcObject {
    
  public:
    
    virtual ~DxvkResource();
    
    bool isInUse() const {
      return m_useCount != 0;
    }
    
    void acquire() { m_useCount += 1; }
    void release() { m_useCount -= 1; }
    
  private:
    
    std::atomic<uint32_t> m_useCount = { 0u };
    
  };
  
}