#include "dxvk_cmdlist.h"

namespace dxvk {
    
  DxvkCommandList::DxvkCommandList(
    const Rc<vk::DeviceFn>& vkd,
          uint32_t          queueFamily)
  : m_vkd(vkd) {
    TRACE(this, queueFamily);
    
    VkCommandPoolCreateInfo poolInfo;
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext            = nullptr;
    poolInfo.flags            = 0;
    poolInfo.queueFamilyIndex = queueFamily;
    
    if (m_vkd->vkCreateCommandPool(m_vkd->device(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS)
      throw DxvkError("DxvkCommandList::DxvkCommandList: Failed to create command pool");
    
    VkCommandBufferAllocateInfo cmdInfo;
    cmdInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdInfo.pNext             = nullptr;
    cmdInfo.commandPool       = m_pool;
    cmdInfo.level             = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdInfo.commandBufferCount = 1;
    
    if (m_vkd->vkAllocateCommandBuffers(m_vkd->device(), &cmdInfo, &m_buffer) != VK_SUCCESS)
      throw DxvkError("DxvkCommandList::DxvkCommandList: Failed to allocate command buffer");
  }
  
  
  DxvkCommandList::~DxvkCommandList() {
    TRACE(this);
    m_resources.reset();
    
    m_vkd->vkDestroyCommandPool(
      m_vkd->device(), m_pool, nullptr);
  }
  
  
  void DxvkCommandList::beginRecording() {
    TRACE(this);
    
    VkCommandBufferBeginInfo info;
    info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext            = nullptr;
    info.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    info.pInheritanceInfo = nullptr;
    
    if (m_vkd->vkResetCommandPool(m_vkd->device(), m_pool, 0) != VK_SUCCESS)
      throw DxvkError("DxvkCommandList::beginRecording: Failed to reset command pool");
    
    if (m_vkd->vkBeginCommandBuffer(m_buffer, &info) != VK_SUCCESS)
      throw DxvkError("DxvkCommandList::beginRecording: Failed to begin command buffer recording");
  }
  
  
  void DxvkCommandList::endRecording() {
    TRACE(this);
    
    if (m_vkd->vkEndCommandBuffer(m_buffer) != VK_SUCCESS)
      throw DxvkError("DxvkCommandList::endRecording: Failed to record command buffer");
  }
  
  
  void DxvkCommandList::trackResource(const Rc<DxvkResource>& rc) {
    TRACE(this, rc);
    m_resources.trackResource(rc);
  }
  
  
  void DxvkCommandList::reset() {
    TRACE(this);
    m_resources.reset();
  }
  
  
  void DxvkCommandList::cmdBeginRenderPass(
    const VkRenderPassBeginInfo*  pRenderPassBegin,
          VkSubpassContents       contents) {
    m_vkd->vkCmdBeginRenderPass(m_buffer,
      pRenderPassBegin, contents);
  }
  
  
  void DxvkCommandList::cmdBindPipeline(
          VkPipelineBindPoint     pipelineBindPoint,
          VkPipeline              pipeline) {
    m_vkd->vkCmdBindPipeline(m_buffer,
      pipelineBindPoint, pipeline);
  }
  
  
  void DxvkCommandList::cmdClearAttachments(
          uint32_t                attachmentCount,
    const VkClearAttachment*      pAttachments,
          uint32_t                rectCount,
    const VkClearRect*            pRects) {
    m_vkd->vkCmdClearAttachments(m_buffer,
      attachmentCount, pAttachments,
      rectCount, pRects);
  }
  
  
  void DxvkCommandList::cmdDispatch(
          uint32_t                x,
          uint32_t                y,
          uint32_t                z) {
    m_vkd->vkCmdDispatch(m_buffer, x, y, z);
  }
  
  
  void DxvkCommandList::cmdDraw(
          uint32_t                vertexCount,
          uint32_t                instanceCount,
          uint32_t                firstVertex,
          uint32_t                firstInstance) {
    m_vkd->vkCmdDraw(m_buffer,
      vertexCount, instanceCount,
      firstVertex, firstInstance);
  }
  
  
  void DxvkCommandList::cmdDrawIndexed(
          uint32_t                indexCount,
          uint32_t                instanceCount,
          uint32_t                firstIndex,
          uint32_t                vertexOffset,
          uint32_t                firstInstance) {
    m_vkd->vkCmdDrawIndexed(m_buffer,
      indexCount, instanceCount,
      firstIndex, vertexOffset,
      firstInstance);
  }
  
  
  void DxvkCommandList::cmdEndRenderPass() {
    m_vkd->vkCmdEndRenderPass(m_buffer);
  }
  
}