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
    VE_MeshData m_meshData;     /*!< vulkan GPU memory mesh data*/
    VE_StagingMesh m_staging;   /*!< staging mesh data*/

    /*@brief mesh cpu data to transfer to vulkan and cleared after*/
    VE_Shape<VE_MeshVertex> m_cpuStaticMesh;

    [[nodiscard]] constexpr bool isInputMeshValid()const noexcept
    {
        return !m_cpuStaticMesh.m_vertexBuffer.empty();
    }

    /*@brief transfer index buffer to gpu*/
    void transfertIndexBuffer(const VE_DeviceContext& a_ctx, VkCommandBuffer& a_cmdBuffer);

    /*@brief transfer vertex buffer to gpu*/
    void transfertVertexBuffer(const VE_DeviceContext& a_ctx, VkCommandBuffer& a_cmdBuffer);

    /*@brief release buffers*/
    void releaseBuffers();

protected:
    /*@brief write rendering commands*/
    void writeCommands(VkCommandBuffer& a_cmdBuffer) override;

    /*@brief update call each frame by parent component*/
    void update(const float /*a_elapsed*/) override;

public:
    void setMeshData(const VE_Shape<VE_MeshVertex>& a_meshData);
    // todo
};
