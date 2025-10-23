#pragma once
/***********************************************
* @headerfile VE_RenderGraph.h
* @date 19 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "utils/VulkanContext.h"
#include "rendering/components/VE_Component.h"

class VE_GraphicalPipeline;

/*@brief Manage rendering in multiple threads*/
class VE_RenderGraph //: public VulkanObject<VE_DeviceContext>
{
private:
    std::unordered_map<uint32_t, std::shared_ptr<VE_GraphicalPipeline>> m_registeredPipelines;          /*!< pipelines per Id*/
    std::unordered_map<uint32_t, std::vector<std::weak_ptr<VE_Component>>> m_componentByPipelineId;     /*!< components per pipeline id (component rendereing use associated pipeline)*/

    struct CommandThread
    {
        std::thread m_thread;                   /*!< working thread*/
        VkCommandBuffer m_cmdBuffer;            /*!< command buffer used in the thread*/
        std::vector<uint32_t> m_pipelinesIds;   /*!< pipelines to process*/
    };
    std::vector<CommandThread> m_commandThreadPool;                                                       /*!< threads working on different command buffers*/

public:
    [[nodiscard]] bool registerComponent(const std::shared_ptr<VE_Component>& a_component);
    // todo
};