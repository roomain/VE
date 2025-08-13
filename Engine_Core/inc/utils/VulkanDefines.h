#pragma once
#include <vector>
#include <array>
#include <vulkan/vulkan.hpp>

using VkCommandBufferVector = std::vector<VkCommandBuffer>;
template<std::size_t Count>
using  VkCommandBufferVectorArray = std::array<VkCommandBuffer, Count>;


using  SemaphoreVector = std::vector<VkSemaphore>;
template<std::size_t Count>
using  SemaphoreArray = std::array<VkSemaphore, Count>;

using VertexInputBindingDescVector = std::vector<VkVertexInputBindingDescription>;
template<std::size_t Count>
using VertexInputBindingDescArray = std::array<VkVertexInputBindingDescription, Count>;

using VertexInputAttribDescVector = std::vector<VkVertexInputAttributeDescription>;
template<std::size_t Count>
using VertexInputAttribDescArray = std::array<VkVertexInputAttributeDescription, Count>;


struct DepthStencilSettings
{
	VkBool32 depthTestEnable = VK_TRUE;
	VkBool32 depthWriteEnable = VK_TRUE;
	VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
};

struct RasterizationSettings
{
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
	VkCullModeFlags cullFlags = VK_CULL_MODE_FRONT_BIT;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
};