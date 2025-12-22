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

void VE_RenderGraphTask::process(PipelineDatabase& a_pipelineData, OnlyOneTime<InitBufferAction>& a_bufferInit)
{
    uint32_t indexToRemove = 0;
    std::vector<uint32_t> listToRemove;
    for (const auto& componentWkPtr : a_pipelineData.components)
    {
        if (auto cmpPtr = componentWkPtr.lock())
        {
            if (cmpPtr->hasFlag<RenderingFlagBit::IS_RENDERING>() && !cmpPtr->hasFlag<RenderingFlagBit::IS_EDITABLE>())
            {
                a_bufferInit();
                cmpPtr->writeCommands(m_cmdBuffer);
            }
        }
        else
        {
            listToRemove.emplace_back(indexToRemove);
        }
        ++indexToRemove;
    }

    uint32_t offset = 0;
    for (const auto index : listToRemove)
    {
        a_pipelineData.components.erase(
            a_pipelineData.components.begin() + (index - offset));
        offset++;
    }
}

void VE_RenderGraphTask::process(const VE_RenderingScenePtr& a_data)
{
    // todo if no change only flush

    if (m_cmdBuffer == VK_NULL_HANDLE)
        return;
    
    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);

    m_noRendering = true;
    OnlyOneTime<InitBufferAction> initBuffer{
        [this, &cmbBeginInfo]()
        {
            m_noRendering = false;
            vkResetCommandBuffer(m_cmdBuffer, 0);
            vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
        }
    };

    if (m_pipelineToRender.empty())
    {
        // do all pipelines
        for (auto&[pipeline, data] : a_data->componentsPerPipeline)
        {
            if (data.bUpdated)
            {
                process(data, initBuffer);
            }
        }
    }
    else
    {
        for (auto pipeline : m_pipelineToRender)
        {
            if (auto pipelineDataIter = a_data->componentsPerPipeline.find(pipeline);
                pipelineDataIter != a_data->componentsPerPipeline.end() && pipelineDataIter->second.bUpdated)
            {
                process(pipelineDataIter->second, initBuffer);
            }
        }
    }

    if(initBuffer.isExecuted())
        vkEndCommandBuffer(m_cmdBuffer);
}

void VE_RenderGraphEditTask::process(const VE_RenderingScenePtr& a_data)
{
    m_noRendering = true;
    if (m_cmdBuffer == VK_NULL_HANDLE || !a_data->hasEditComponent())
        return;

    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);
    

    OnlyOneTime<InitBufferAction> initBuffer{
        [this, &cmbBeginInfo]()
        {
            m_noRendering = false;
            vkResetCommandBuffer(m_cmdBuffer, 0);
            vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
        }
    };

    for (const auto& componentWk : a_data->editedComponent)
    {
        if (auto component = componentWk.lock())
        {
            component->pipeline()->bind(m_cmdBuffer);
            if (component->hasFlag<RenderingFlagBit::IS_RENDERING>() && component->hasFlag<RenderingFlagBit::IS_EDITABLE>())
            {
                initBuffer();
                component->writeCommands(m_cmdBuffer);
            }
        }
    }

    if (initBuffer.isExecuted())
        vkEndCommandBuffer(m_cmdBuffer);
}