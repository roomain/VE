#pragma once
/***********************************************
* @headerfile VE_MeshComponent.h
* @date 19 / 10 / 2025
* @author Roomain
************************************************/
#include "rendering/components/VE_Component.h"

class VE_RenderGraph;

class VE_MeshComponent : public VE_Component
{
private:
    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;    /*!< mesh vertex contains vertex and normals*/
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;     /*!< mesh index buffer */

    // todo

protected:
    /*@brief write rendering commands*/
    virtual void writeCommands(VkCommandBuffer& a_cmdBuffer)const = 0;

    /*@brief update call each frame by parent component*/
    virtual void update(const float a_elapsed) = 0;

public:
    /*@brief register component to render scene*/
    [[nodiscard]] bool registerComponent(VE_RenderGraph& a_graph) override;
};
