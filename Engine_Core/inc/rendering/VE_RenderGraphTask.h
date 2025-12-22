#pragma once
/***********************************************
* @headerfile VE_RenderGraphTask.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <vector>
#include <list>
#include <functional>
#include <vulkan/vulkan.hpp>
#include "TGroupedTaskInstance.h"
#include "utils/VulkanContext.h"
#include "rendering/VE_RenderingScene.h"
#include "OnlyOneTime.h"

class VE_RenderGraph;

using InitBufferAction = std::function<void()>;

/*@brief interface for rendering task*/
class VE_IRenderGraphTask : public TGroupedTaskInstance<VE_RenderingScenePtr>
{
protected:
    VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;       /*!< command buffer managed by task manager*/
    bool m_noRendering = true;                          /*! indicate if no rendering eg no data to process*/

public:
    explicit VE_IRenderGraphTask(TaskSynchroPtr a_pSynchro);
    virtual ~VE_IRenderGraphTask() = default;
    void setCmdBuffer(VkCommandBuffer a_cmdBuffer) { m_cmdBuffer = a_cmdBuffer; }
    [[nodiscard]] bool noRendering()const { return m_noRendering; }
    virtual void process(const VE_RenderingScenePtr& a_data) = 0;
};

class VE_RenderGraphTask : public VE_IRenderGraphTask
{
private:
    std::vector<VE_GraphicalPipelinePtr> m_pipelineToRender; /*!< list of pipelines to process*/
    void process(PipelineDatabase& a_pipelineData, OnlyOneTime<InitBufferAction>& a_bufferInit);

public:
    using VE_IRenderGraphTask::VE_IRenderGraphTask;
    ~VE_RenderGraphTask()override = default;
    void process(const VE_RenderingScenePtr& a_data) override;
    constexpr std::vector<VE_GraphicalPipelinePtr>::const_iterator cbegin()const { return m_pipelineToRender.cbegin(); }
    constexpr std::vector<VE_GraphicalPipelinePtr>::const_iterator cend()const { return m_pipelineToRender.cend(); }
};

class VE_RenderGraphEditTask : public VE_IRenderGraphTask
{
public:
    using VE_IRenderGraphTask::VE_IRenderGraphTask;
    ~VE_RenderGraphEditTask()override = default;
    void process(const VE_RenderingScenePtr& a_data) final;
};