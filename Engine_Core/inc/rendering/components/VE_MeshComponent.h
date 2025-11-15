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
#include "Memory/VE_Memory.h"

class VE_RenderGraph;


/*@brief represents a static mesh component*/
class VE_StaticMeshComponent : public VE_Component<VE_MeshPipeline>
{
    //DECLARE_PIPELINE(VE_MeshPipeline)

private:
    VulkanBuffer m_vertexBuffer;   /*!< vulkan buffer mesh vertex contains vertex and normals*/
    VulkanBuffer m_indexBuffer;    /*!< vulkan buffer mesh index buffer */
    uint32_t m_indexCount = 0;                  /*!< number of elements in index buffer*/
    /*@brief mesh cpu data to transfer to vulkan and cleared after*/
    VE_Shape<VE_MeshVertex> m_cpuStaticMesh;

    [[nodiscard]] constexpr bool isInputMeshValid()const noexcept
    {
        return !m_cpuStaticMesh.m_vertexBuffer.empty();
    }

protected:
    /*@brief write rendering commands*/
    virtual void writeCommands(VkCommandBuffer& a_cmdBuffer) override;

    /*@brief update call each frame by parent component*/
    virtual void update(const float /*a_elapsed*/) override;

public:
    // todo
};
