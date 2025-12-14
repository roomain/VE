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
#include "TGroupedTaskManager.h"
#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_IComponent.h"

/*@brief Manage rendering in multiple threads*/
class VE_RenderGraph : public VulkanObject<VE_DeviceContext>, 
    public TGroupedTaskManager<VE_GraphData, VE_RenderGraphTask>
{
private:
    VE_GraphData m_renderGraph;


public:
    VE_RenderGraph(const VE_DeviceContext& a_ctxt);
    ~VE_RenderGraph();
    [[nodiscard]] bool registerComponent(const std::shared_ptr<VE_IComponent>& a_component);
    // todo
};