#pragma once
/***********************************************
* @headerfile VE_RenderGraphTask.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <list>
#include <functional>
#include <vulkan/vulkan.hpp>
#include "TGroupedTaskInstance.h"
#include "utils/VulkanContext.h"
#include "rendering/VE_RenderingScene.h"
#include "TOnlyOneTime.h"

class VE_RenderGraph;

using InitBufferAction = std::function<void()>;

struct VE_TaskParameters
{
    VE_RenderingScenePtr sharedData;
    VkCommandBuffer workingBuffer;
};

struct VE_TaskParametersEx : public VE_TaskParameters
{
    std::vector<VE_PipelinePtr> workingPipelines;
};

/*@brief interface for rendering task*/
class VE_IRenderGraphTask : public TGroupedTaskInstance<VE_RenderingScenePtr>
{
protected:
    VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;       /*!< command buffer managed by task manager*/
    bool m_noRendering = true;                          /*! indicate if no rendering eg no data to process*/

public:
    explicit VE_IRenderGraphTask(TaskSynchroPtr a_pSynchro);
    explicit VE_IRenderGraphTask(TaskSynchroPtr a_synchro, const VE_TaskParameters& a_parameters);
    virtual ~VE_IRenderGraphTask() = default;
    void setCmdBuffer(VkCommandBuffer a_cmdBuffer) { m_cmdBuffer = a_cmdBuffer; }
    [[nodiscard]] bool noRendering()const { return m_noRendering; }
    virtual void process(const VE_RenderingScenePtr& a_data) = 0;
    [[nodiscard]] VkCommandBuffer commandBuffer()const { return m_cmdBuffer; }
};

class VE_RenderGraphTask : public VE_IRenderGraphTask
{
private:
	mutable std::mutex m_pipelineListProtect;                /*!< protect pipeline list*/
    std::vector<VE_PipelinePtr> m_pipelineToProcess; /*!< list of pipelines to process*/
    bool processComponent(VE_IComponentWPtr& a_component, TOnlyOneTime<InitBufferAction>& a_bufferInit);
    void clean(const VE_RenderingScenePtr& a_renderingScene, const VE_PipelinePtr a_pipeline, const std::vector<uint32_t>& listToRemove)const;

public:
    using VE_IRenderGraphTask::VE_IRenderGraphTask;
    explicit VE_RenderGraphTask(TaskSynchroPtr a_synchro, const VE_TaskParametersEx& a_parameters);
    ~VE_RenderGraphTask()override = default;
    void addPipelineToProcess(const VE_PipelinePtr& a_pipeline);
    void addPipelinesToProcess(const std::vector<VE_PipelinePtr>& a_pipelines);
    constexpr size_t pipelineToProcessCount()const 
    { 
        std::scoped_lock lock(m_pipelineListProtect);
        return m_pipelineToProcess.size(); 
	}
    void clearPipelinesToProcess();
    void process(const VE_RenderingScenePtr& a_renderingScene) override;
    constexpr std::vector<VE_PipelinePtr>::const_iterator cbegin()const { return m_pipelineToProcess.cbegin(); }
    constexpr std::vector<VE_PipelinePtr>::const_iterator cend()const { return m_pipelineToProcess.cend(); }
};

class VE_RenderGraphEditTask : public VE_IRenderGraphTask
{
public:
    using VE_IRenderGraphTask::VE_IRenderGraphTask;
    ~VE_RenderGraphEditTask()override = default;
    void process(const VE_RenderingScenePtr& a_renderingScene) final;
};