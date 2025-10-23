#include "pch.h"
#include <array>
#include "memory/VE_Memory.h"
#include "rendering/components/VE_MeshComponent.h"

//IMPL_PIPELINE(VE_MeshPipeline, VE_StaticMeshComponent)

void VE_StaticMeshComponent::writeCommands(const VE_DeviceContext& a_vkCtx, VkCommandBuffer& a_cmdBuffer)const
{
	if (m_vertexBuffer != VK_NULL_HANDLE && m_indexBuffer != VK_NULL_HANDLE)
	{
		static const std::array<VkDeviceSize, 1> s_size = { 0 };
		vkCmdBindVertexBuffers(a_cmdBuffer, 0, 1, &m_vertexBuffer, s_size.data());
		vkCmdBindIndexBuffer(a_cmdBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(a_cmdBuffer, m_indexCount, 1, 0, 0, 0);
	}
	else
	{
		if (isInputMeshValid())
		{
			const auto stagingVBuffer = AllocateAndWriteStagingBuffer(a_vkCtx, m_cpuStaticMesh.m_vertexBuffer);
			const auto stagingIBuffer = AllocateAndWriteStagingBuffer(a_vkCtx, m_cpuStaticMesh.m_indexBuffer);

			//VK_CHECK_EXCEPT(vkMapMemory(a_vkCtx.m_logicalDevice, ))
			// todo transfer
		}
	}
}

void VE_StaticMeshComponent::update(const float /*a_elapsed*/)
{
	//todo
}
