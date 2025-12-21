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
using VectorOfWkComponents = std::vector<VE_IComponentWPtr>;

struct PipelineDatabase
{
    bool bUpdated = false;          /*!< indicate if components list is modified*/
    VectorOfWkComponents components;/*!< list of components sharing the same pipeline*/
};

using ComponentsDatabase = std::unordered_map<VE_GraphicalPipelinePtr, PipelineDatabase>;
using ComponentCallback = std::function<void(const VE_IComponentWPtr&)>;

/*
* Scene components are rendered by multiple command buffers in multiple tasks
* each tasks is responsible of rendering one or multiple pipelines eg all components 
* sharing the same pipeline are rendered in the same tasks (= in same command buffer)
* Edited components are rendered in a specific task.
* Then non edition task don't need to update command buffer at each frame.
*/

/*@brief regroup all rendering component in scene*/
class VE_RenderingScene : public VulkanObject<VE_DeviceContext>
{
    VE_DeviceContext m_renderCtx;
    /*An edited component is not in m_ComponentsPerPipeline*/
    ComponentsDatabase m_ComponentsPerPipeline; /*!< rendering components per pipeline*/
    VectorOfWkComponents m_editedComponent;     /*!< components in edition mode*/
    
    inline void forEach(const VectorOfWkComponents& a_componentList, const ComponentCallback& a_fun)const
    {        
        for(const auto& component : a_componentList)
        {
            a_fun(component);
        }
    }

public:
    VE_RenderingScene(const VE_DeviceContext& a_ctxt) : VulkanObject<VE_DeviceContext>(a_ctxt) {}
    ~VE_RenderingScene() override = default;

    inline bool hasEditComponent()const { return m_editedComponent.empty(); }
    void setDeviceCtx(const VE_DeviceContext& a_devCtx){ m_renderCtx = a_devCtx; }
    inline VE_DeviceContext deviceCtx()const { return m_renderCtx; }
    inline void editComponent(const VE_IComponentWPtr& a_editComponent)
    {
        a_editComponent.lock()->setRenderFlag(RenderingFlagBit::IS_EDITABLE);
        m_editedComponent.emplace_back(a_editComponent);
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
        m_editedComponent.erase(std::ranges::find_if(m_editedComponent, 
            [&componentPtr](const auto& a_comp) { return a_comp.lock() == componentPtr; }));
        auto pipeline = componentPtr->pipeline();
        m_ComponentsPerPipeline.at(pipeline).bUpdated = true;

    }

    inline void unEditComponents(const VectorOfWkComponents& a_editComponent)
    {
        for(const auto& component : a_editComponent)
        {
            unEditComponent(component);
        }
    }

    inline void forEach(const VE_GraphicalPipelinePtr& a_pipeline, const ComponentCallback& a_fun)const
    {        
        if (auto componentListIter = m_ComponentsPerPipeline.find(a_pipeline); componentListIter != m_ComponentsPerPipeline.end())
        {
            if(componentListIter->second.bUpdated)
                forEach(componentListIter->second.components, a_fun);
        }
    }

    inline void forEachEdit(const ComponentCallback& a_fun)const
    {        
        forEach(m_editedComponent, a_fun);
    }

    inline void remove(const VE_GraphicalPipelinePtr& a_pipeline, const std::vector<uint32_t>& a_indexToRemove)
    {
        if (auto componentListIter = m_ComponentsPerPipeline.find(a_pipeline); componentListIter != m_ComponentsPerPipeline.end())
        {
            uint32_t offset = 0;
            for (const auto index : a_indexToRemove)
            {
                componentListIter->second.components.erase(componentListIter->second.components.begin() + (index - offset));
                ++offset;
            }
        }
    }
};

using VE_RenderingScenePtr = std::shared_ptr<VE_RenderingScene>;