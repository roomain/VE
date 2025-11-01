#pragma once
/***********************************************
* @headerfile VE_RenderGraphTask.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "TGroupedTaskInstance.h"
#include "utils/VulkanContext.h"

class VE_GraphicalPipeline;
class VE_Component;

using  VE_GraphicalPipelinePtr = std::shared_ptr<VE_GraphicalPipeline>;
using VectorOfWkComponents = std::vector<std::weak_ptr<VE_Component>>;

struct VE_GraphData
{
    VE_DeviceContext m_renderCtx;
    std::unordered_map<uint32_t, VE_GraphicalPipelinePtr> m_registeredPipelines;/*!< pipelines per Id*/
    std::unordered_map<uint32_t, VectorOfWkComponents> m_componentByPipelineId; /*!< components per pipeline id (component rendereing use associated pipeline)*/
};

class VE_RenderGraph;

class VE_RenderGraphTask : public TGroupedTaskInstance<VE_GraphData>
{
private:
    VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;
    std::vector<uint32_t> m_pipelineIds;

public:
    void process(const VE_GraphData& a_data);
};


