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
#include "rendering/VE_RenderingScene.h"

struct RenderGraphConfiguration
{
    const VkQueue renderQueue;  /*!< queue used for rendering*/
};

/*@brief Manage rendering in multiple threads*/
class VE_RenderGraph : public VulkanObject<VE_DeviceContext>, 
    public TGroupedTaskManager<VE_RenderingScenePtr, VE_IRenderGraphTask>
{
private:
    static constexpr unsigned int m_editTaskIndex = 0;  /*!< index of edit task*/
    VkQueue m_renderQueue;                              /*!< render queue*/
    VE_RenderingScenePtr m_renderScene;                 /*!< data shared between tasks*/
    VkSubmitInfo m_submitInfo;                          /*!< submit information*/
    VkCommandBuffer m_mainCmdBuffer = VK_NULL_HANDLE;   /*!< command buffer managed by task manager*/

protected:
    void startProcess() override;
    void finishProcess() override;

public:
    VE_RenderGraph() = delete;
    explicit VE_RenderGraph(const VE_DeviceContext& a_ctxt, const VkQueue a_renderQueue);
    [[nodiscard]] bool registerComponent(const std::shared_ptr<VE_IComponent>& a_component);

    /*@brief */
    //void setupTasks(const uint32_t)
    // todo
};