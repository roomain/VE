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

using VE_GraphicalPipelinePtr = std::shared_ptr<VE_GraphicalPipeline>;
using VE_IComponentWPtr = std::weak_ptr<VE_IComponent>;
using VE_IComponentPtr = std::shared_ptr<VE_IComponent>;
using VectorOfWkComponents = std::vector<VE_IComponentWPtr>;

struct PipelineDatabase
{
    bool bUpdated = false;          /*!< indicate if components list is modified*/
    VectorOfWkComponents components;/*!< list of components sharing the same pipeline*/
};

using ComponentsDatabase = std::unordered_map<VE_GraphicalPipelinePtr, PipelineDatabase>;
using ComponentCallback = std::function<void(const VE_IComponentPtr&)>;

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
    VE_DeviceContext renderCtx;
    /*An edited component is not in m_ComponentsPerPipeline*/
    ComponentsDatabase componentsPerPipeline; /*!< rendering components per pipeline*/
    VectorOfWkComponents editedComponent;     /*!< components in edition mode*/

    inline bool hasEditComponent()const { return editedComponent.empty(); }
    inline void editComponent(const VE_IComponentWPtr& a_editComponent)
    {
        a_editComponent.lock()->setRenderFlag(RenderingFlagBit::IS_EDITABLE);
        editedComponent.emplace_back(a_editComponent);
    }

    inline void editComponents(const VectorOfWkComponents& a_editComponent)
    {
        for(const auto& component : a_editComponent)
        {
            editComponent(component);
        }
    }

    inline void unEditComponent(const VE_IComponentWPtr& a_editComponent)
    {
        const auto componentPtr = a_editComponent.lock();
        componentPtr->removeRenderFlag(RenderingFlagBit::IS_EDITABLE);
        editedComponent.erase(std::ranges::find_if(editedComponent, 
            [&componentPtr](const auto& a_comp) { return a_comp.lock() == componentPtr; }));
        auto pipeline = componentPtr->pipeline();
        componentsPerPipeline.at(pipeline).bUpdated = true;

    }

    inline void unEditComponents(const VectorOfWkComponents& a_editComponent)
    {
        for(const auto& component : a_editComponent)
        {
            unEditComponent(component);
        }
    }
};

using VE_RenderingScenePtr = std::shared_ptr<VE_RenderingScene>;