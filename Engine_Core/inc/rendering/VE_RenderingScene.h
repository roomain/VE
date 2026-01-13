#pragma once
/***********************************************
* @headerfile VE_RenderingScene.h
* @date 19 / 12 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <ranges>
#include <vulkan/vulkan.hpp>
#include "rendering/components/VE_IComponent.h"
#include "utils/VulkanContext.h"
#include "VE_Pipeline.h"
#include "TDatabase.h"

class VE_Camera;

using VE_PipelinePtr = std::shared_ptr<VE_Pipeline>;
using VE_IComponentWPtr = std::weak_ptr<VE_IComponent>;
using VE_IComponentPtr = std::shared_ptr<VE_IComponent>;
using VectorOfWkComponents = std::vector<VE_IComponentWPtr>;
using VE_CameraPtr = std::shared_ptr<VE_Camera>;
using ComponentsDatabase = TDatabase<VE_PipelinePtr, bool, VE_IComponentWPtr>;
using ComponentFilter = std::function<bool(const VE_IComponentPtr&)>;

/*@brief context for rendering scene*/
struct VE_SceneContext
{
	VE_CameraPtr camera;                    /*!< rendering camera*/
	VkViewport viewport;                    /*!< rendering viewport*/
};

/*
* Scene components are rendered by multiple command buffers in multiple tasks
* each tasks is responsible of rendering one or multiple pipelines eg all components 
* sharing the same pipeline are rendered in the same tasks (= in same command buffer)
* Edited components are rendered in a specific task.
* Then non edition task don't need to update command buffer at each frame.
*/

/*@brief regroup all rendering component in scene*/
struct VE_RenderingScene
{
    std::mutex renderingProtect;                    /*!< mutex to synchronise rendering/select component */
	std::shared_ptr<VE_SceneContext> sceneContext;  /*!< rendering scene context*/
    /*An edited component is not in m_ComponentsPerPipeline*/
    ComponentsDatabase componentsPerPipeline;       /*!< rendering components per pipeline*/
    VectorOfWkComponents editedComponent;           /*!< components in edition mode ordered by pipeline*/

#pragma region Slots
#pragma warning(push)
#pragma warning( disable : 4100 4189) // unused local variable lock
    inline void onRenderFlagChanged(const VE_IComponent* a_component, [[maybe_unused]]const RenderingFlagBit, [[maybe_unused]] const RenderingFlagBit)
    {
        if (auto pipeline = a_component->pipeline(); a_component->isRegistered() && pipeline)
        {
            auto&& lock = pipeline->scopeLock();
            componentsPerPipeline.updateColumnData(pipeline, true);
        }
    }

    inline void onInvalidate(const VE_IComponent* a_component)
    {
        if (auto pipeline = a_component->pipeline(); a_component->isRegistered() && pipeline)
        {
            auto&& lock = pipeline->scopeLock();
            componentsPerPipeline.updateColumnData(pipeline, true);
        }
    }

    inline void onSelect(VE_IComponent* a_component, bool a_selected)
    {
        if (auto pipeline = a_component->pipeline(); a_component->isRegistered() && pipeline)
        {
            auto&& lock = pipeline->scopeLock();
            componentsPerPipeline.updateColumnData(pipeline, true);
            std::scoped_lock selectLock(renderingProtect);
            if (a_selected)
            {
                if (editedComponent.empty())
                {
                    editedComponent.emplace_back(a_component->weak_from_this());
                }
                else
                {
                    // order by pipeline
                    if (auto iter = std::ranges::find_if(editedComponent, [pipeline](auto& a_cmpWk)
                        {
                            if (auto cmp = a_cmpWk.lock())
                            {
                                return cmp->pipeline() == pipeline;
                            }
                            return false;
                        }); iter != editedComponent.cend())
                    {
                        editedComponent.insert(iter, a_component->weak_from_this());
                    }
                    else
                    {
                        editedComponent.emplace_back(a_component->weak_from_this());
                    }
                }
                // todo order by pipeline
            }
            else if(auto iter = std::ranges::find_if(editedComponent, [a_component](auto&& a_comp)
                {
                    return a_comp.lock().get() == a_component;
                }); iter != editedComponent.end())
            {
                editedComponent.erase(iter);
            }

        }
    }
#pragma warning(pop)

#pragma region
    inline bool hasEditComponent()const 
    {
        return editedComponent.empty(); 
    }
};

using VE_RenderingScenePtr = std::shared_ptr<VE_RenderingScene>;