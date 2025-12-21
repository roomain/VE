#include "pch.h"
#include <functional>
#include "rendering/pipelines/VE_GraphicalPipeline.h"
#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_IComponent.h"
#include "utils/VulkanCmdInitializers.h"

VE_IRenderGraphTask::VE_IRenderGraphTask(TaskSynchroPtr a_pSynchro) : TGroupedTaskInstance<VE_RenderingScenePtr>{ a_pSynchro }
{
    setCallback(std::bind_front(&VE_IRenderGraphTask::process, this));
}



void VE_RenderGraphTask::process(const VE_RenderingScenePtr& a_data)
{
    // todo if no change only flush

    if (m_cmdBuffer == VK_NULL_HANDLE)
        return;
    
    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);
    
    vkResetCommandBuffer(m_cmdBuffer, 0);
    vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
    for (auto pipeline : m_pipelineToRender)
    {
        uint32_t index = 0;
        std::vector<uint32_t> removed; // indices of released components 
        a_data->forEach(pipeline, [this, &index, &removed](const auto& a_cmpWptr)
            {
                if (auto component = a_cmpWptr.lock())
                {
                    component->pipeline()->bind(m_cmdBuffer);
                    if (component->hasFlag<RenderingFlagBit::IS_RENDERING>())
                    {
                        m_noRendering = false;
                        component->writeCommands(m_cmdBuffer);
                    }
                    else
                    {
                        // add released component to remove list
                        removed.emplace_back(index);
                    }
                    ++index;
                }
            });

        // erase release components
        if(!removed.empty())
            a_data->remove(pipeline, removed);
    }
    vkEndCommandBuffer(m_cmdBuffer);
}

void VE_RenderGraphEditTask::process(const VE_RenderingScenePtr& a_data)
{
    m_noRendering = true;
    if (m_cmdBuffer == VK_NULL_HANDLE || !a_data->hasEditComponent())
        return;

    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);
    vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);

    a_data->forEachEdit([this](const auto& a_cmpWptr)
        {
            if (auto component = a_cmpWptr.lock())
            {
                component->pipeline()->bind(m_cmdBuffer);
                if (component->hasFlag<RenderingFlagBit::IS_RENDERING>())
                {
                    m_noRendering = false;
                    component->writeCommands(m_cmdBuffer);
                }
            }
        });
    vkEndCommandBuffer(m_cmdBuffer);
}