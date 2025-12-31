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

bool VE_RenderGraphTask::processComponent(VE_IComponentWPtr& a_component, TOnlyOneTime<InitBufferAction>& a_bufferInit)
{
    
    if (auto cmpPtr = a_component.lock())
    {
        if (cmpPtr->hasFlag<RenderingFlagBit::IS_RENDERING>() && !cmpPtr->hasFlag<RenderingFlagBit::IS_EDITABLE>())
        {
            a_bufferInit();
            cmpPtr->writeCommands(m_cmdBuffer);
        }
        return true;
    }
    return false;
}

void VE_RenderGraphTask::clean(const VE_RenderingScenePtr& a_renderingScene, const VE_PipelinePtr a_pipeline, const std::vector<uint32_t>& listToRemove)const
{
    uint32_t offset = 0;
    for (const auto index : listToRemove)
    {
        a_renderingScene->componentsPerPipeline.remove(a_pipeline, index - offset);
        offset++;
    }
}

#pragma warning(push)
#pragma warning( disable : 4100 4189)
void VE_RenderGraphTask::process(const VE_RenderingScenePtr& a_renderingScene)
{
    // todo if no change only flush
    if (m_cmdBuffer == VK_NULL_HANDLE)
        return;
    
    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);

    m_noRendering = true;
    TOnlyOneTime<InitBufferAction> initBuffer{
        [this, &cmbBeginInfo]()
        {
            m_noRendering = false;
            vkResetCommandBuffer(m_cmdBuffer, 0);
            vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
        }
    };

    if (m_pipelineToProcess.empty())
    {
        // do all pipelines
        a_renderingScene->componentsPerPipeline.for_all([&, this](auto& a_pipeline, auto& a_componentList)
            {
                uint32_t indexToRemove = 0;
                std::vector<uint32_t> listToRemove;
                auto&& lock = a_pipeline->scopeLock();
                
                for (auto& component : a_componentList)
                {
                    if (processComponent(component, initBuffer))
                        listToRemove.emplace_back(indexToRemove);
                    ++indexToRemove;
                }

                clean(a_renderingScene, a_pipeline, listToRemove);
            });

    }
    else
    {
        for (auto pipeline : m_pipelineToProcess)
        {
            uint32_t indexToRemove = 0;
            std::vector<uint32_t> listToRemove;
            auto&& lock = pipeline->scopeLock();
            a_renderingScene->componentsPerPipeline.for_each(pipeline, [&, this](auto& a_component)
                {
                    if (processComponent(a_component, initBuffer))
                        listToRemove.emplace_back(indexToRemove);
                    ++indexToRemove;
                });

            clean(a_renderingScene, pipeline, listToRemove);
        }
    }

    if(initBuffer.isExecuted())
        vkEndCommandBuffer(m_cmdBuffer);
}
#pragma warning(pop)

void VE_RenderGraphEditTask::process(const VE_RenderingScenePtr& a_renderingScene)
{
    m_noRendering = true;
    if (m_cmdBuffer == VK_NULL_HANDLE || !a_renderingScene->hasEditComponent())
        return;

    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(0, nullptr);
    

    TOnlyOneTime<InitBufferAction> initBuffer{
        [this, &cmbBeginInfo]()
        {
            m_noRendering = false;
            vkResetCommandBuffer(m_cmdBuffer, 0);
            vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
        }
    };

    std::scoped_lock selectLock(a_renderingScene->renderingProtect);

    for (const auto& componentWk : a_renderingScene->editedComponent)
    {
        if (auto component = componentWk.lock())
        {
            component->pipeline()->bind(m_cmdBuffer);
            if (component->hasFlag<RenderingFlagBit::IS_RENDERING>())
            {
                initBuffer();
                component->writeCommands(m_cmdBuffer);
            }
        }
    }

    if (initBuffer.isExecuted())
        vkEndCommandBuffer(m_cmdBuffer);
}