#include "pch.h"
#include <array>
#include "memory/VE_Memory.h"
#include "rendering/components/VE_MeshComponent.h"

//IMPL_PIPELINE(VE_MeshPipeline, VE_StaticMeshComponent)

void VE_StaticMeshComponent::writeCommands(VkCommandBuffer& a_cmdBuffer)
{
	if (m_vertexBuffer.m_buffer != VK_NULL_HANDLE && m_indexBuffer.m_buffer != VK_NULL_HANDLE)
	{
		static const std::array<VkDeviceSize, 1> s_size = { 0 };
		vkCmdBindVertexBuffers(a_cmdBuffer, 0, 1, &m_vertexBuffer.m_buffer, s_size.data());
		vkCmdBindIndexBuffer(a_cmdBuffer, m_indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(a_cmdBuffer, m_indexCount, 1, 0, 0, 0);
	}
	else if (isInputMeshValid())
	{
		const VE_DeviceContext ctx = context();
		//create and fill staging buffers
		auto stagingVBuffer = AllocateAndWriteStagingBuffer(ctx, m_cpuStaticMesh.m_vertexBuffer);
		auto stagingIBuffer = AllocateAndWriteStagingBuffer(ctx, m_cpuStaticMesh.m_indexBuffer);

		// create GPU device buffers
		const VE_AllocationParameter indexAlloc{
			.m_size = m_cpuStaticMesh.indexBufferMemorySize(),
			.m_usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			.m_allocFlag = 0
		};
		const VE_AllocationParameter vertexAlloc{
			.m_size = m_cpuStaticMesh.vertexBufferMemorySize(),
			.m_usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.m_allocFlag = 0
		};


		m_indexBuffer = allocateBuffer(ctx, indexAlloc);
		m_vertexBuffer = allocateBuffer(ctx, vertexAlloc);

		// copy buffers
		const VkBufferCopy vertexRegion
		{
			.srcOffset = 0,
			.dstOffset = 0,
			.size = m_cpuStaticMesh.vertexBufferMemorySize(),
		};
		const VkBufferCopy indexRegion
		{
			.srcOffset = 0,
			.dstOffset = 0,
			.size = m_cpuStaticMesh.indexBufferMemorySize(),
		};

		vkCmdCopyBuffer(a_cmdBuffer, stagingVBuffer.m_buffer, m_indexBuffer.m_buffer, 1, &vertexRegion);
		vkCmdCopyBuffer(a_cmdBuffer, stagingIBuffer.m_buffer, m_indexBuffer.m_buffer, 1, &indexRegion);

		//release staging buffers
		releaseBuffer(ctx, stagingIBuffer);
		releaseBuffer(ctx, stagingVBuffer);

	}
	// to do release buffer
}

void VE_StaticMeshComponent::update(const float /*a_elapsed*/)
{
	//todo
}
