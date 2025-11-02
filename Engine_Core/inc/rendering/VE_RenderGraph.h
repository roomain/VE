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
#include "rendering/components/VE_Component.h"

/*@brief Manage rendering in multiple threads*/
class VE_RenderGraph : public VulkanObject<VE_DeviceContext>, 
    public TGroupedTaskManager<VE_GraphData, VE_RenderGraphTask>
{
private:
    VE_GraphData m_renderGraph;

public:
    VE_RenderGraph();
    ~VE_RenderGraph();
    [[nodiscard]] bool registerComponent(const std::shared_ptr<VE_Component>& a_component);
    // todo
};