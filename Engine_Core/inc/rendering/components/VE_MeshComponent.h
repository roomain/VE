#pragma once
/***********************************************
* @headerfile VE_MeshComponent.h
* @date 19 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include "rendering/VE_VertexTypes.h"
#include "rendering/components/VE_Component.h"
#include "rendering/pipelines/VE_MeshPipeline.h"

class VE_RenderGraph;


/*@brief represents a static mesh component*/
class VE_StaticMeshComponent : public VE_Component<VE_MeshPipeline>
{
    //DECLARE_PIPELINE(VE_MeshPipeline)

private:
    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;   /*!< vulkan buffer mesh vertex contains vertex and normals*/
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;    /*!< vulkan buffer mesh index buffer */
    uint32_t m_indexCount = 0;                  /*!< number of elements in index buffer*/
    /*@brief mesh cpu data to transfer to vulkan and cleared after*/
    VE_Shape<VE_MeshVertex> m_cpuStaticMesh;

    [[nodiscard]] constexpr bool isInputMeshValid()const noexcept
    {
        return !m_cpuStaticMesh.m_vertexBuffer.empty();
    }

protected:
    /*@brief write rendering commands*/
    virtual void writeCommands(const VE_DeviceContext& a_vkCtx, VkCommandBuffer& a_cmdBuffer)const override;

    /*@brief update call each frame by parent component*/
    virtual void update(const float /*a_elapsed*/) override;

public:
    // todo
};
