#pragma once
/***********************************************
* @headerfile VE_RenderGraph.h
* @date 19 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <functional>
#include <vulkan/vulkan.hpp>
#include "VE_FwdDecl.h"
#include "utils/VulkanContext.h"
#include "TGroupedTaskManager.h"
#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_IComponent.h"
#include "rendering/VE_RenderingScene.h"

using CmdBufferGenerator = std::function<VkCommandBuffer()>;

/*@brief Manage rendering in multiple threads*/
class VE_RenderGraph : public TGroupedTaskManager<VE_RenderingScenePtr, VE_IRenderGraphTask>
{
private:
    bool bHasEditTask = false;                          /*!< has an edition task in position 0*/
    VkQueue m_renderQueue = VK_NULL_HANDLE;             /*!< render queue*/
    VkSubmitInfo m_submitInfo{};                        /*!< submit information*/
    VkCommandBuffer m_mainCmdBuffer = VK_NULL_HANDLE;   /*!< command buffer managed by task manager*/
    std::vector<VkCommandBuffer> m_activeBuffer;        /*!< buffer used for rendering*/

protected:
    void startProcess() override;
    void finishProcess() override;

public:
    VE_RenderGraph() = default;
    ~VE_RenderGraph() override = default;
    void setup(VkQueue a_queue, VkCommandBuffer a_mainCmd);
    [[nodiscard]] inline VkQueue renderQueue()const { return m_renderQueue; }
    [[nodiscard]] bool setEditTask(const VE_TaskParameters a_taskParameters);
    [[nodiscard]] bool addTasks(const std::vector<VE_TaskParametersEx>& a_tasksParameters);
};