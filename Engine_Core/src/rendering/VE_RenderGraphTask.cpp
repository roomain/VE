#include "pch.h"
#include <functional>
#include "rendering/pipelines/VE_GraphicalPipeline.h"
#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_IComponent.h"
#include "utils/VulkanCmdInitializers.h"

VE_RenderGraphTask::VE_RenderGraphTask(TaskSynchroPtr a_pSynchro) : TGroupedTaskInstance<VE_GraphData>{ a_pSynchro }
{
    setCallback(std::bind_front(&VE_RenderGraphTask::process, this));
}

void VE_RenderGraphTask::process(VE_GraphData& a_data)
{
    if (m_cmdBuffer == VK_NULL_HANDLE)
        return;
    
    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(m_usageFlag, nullptr);

    vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
    for (auto pipelineId : m_pipelineIds)
    {
        uint32_t index = 0;
        std::vector<uint32_t> removed; // indices of released components 
        auto pipeline = a_data.m_registeredPipelines.at(pipelineId);
        pipeline->bind(m_cmdBuffer);
        for (auto weakCmp : a_data.m_componentByPipelineId.at(pipelineId))
        {
            if (auto component = weakCmp.lock())
            {
                if (component->hasFlag<RenderingFlagBit::IS_RENDERING>())
                    component->writeCommands(m_cmdBuffer);
            }
            else
            {
                removed.emplace_back(index);
            }
            ++index;
        }

        // erase release components
        uint32_t offset = 0;
        std::reference_wrapper<VectorOfWkComponents> componentList(a_data.m_componentByPipelineId.at(pipelineId));
        for (auto eraseIndex : removed)
        {
            componentList.get().erase(componentList.get().begin() + (eraseIndex - offset));
            offset++;
        }
    }
    vkEndCommandBuffer(m_cmdBuffer);
}
