#include "pch.h"
#include "rendering/VE_UniformSampler.h"


VE_UniformSampler::VE_UniformSampler(const uint32_t a_bindingPoint, const VE_DeviceContext& a_ctxt) : VE_Uniform(a_bindingPoint), VulkanObject<VE_DeviceContext>{ a_ctxt }
{
    //
}

VE_UniformSampler::~VE_UniformSampler()
{
	cleanup();
}

void VE_UniformSampler::initialize()
{
	//todo
}

void VE_UniformSampler::cleanup()
{
	vkDestroySampler(m_vkCtxt.m_logicalDevice, m_sampler, nullptr);
}

void VE_UniformSampler::updateShaderVariable(const VkDescriptorSet a_descSet)
{
	VkDescriptorImageInfo imageInfo{};
	if (m_usage == UseMode::Input)
	{
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.sampler = m_sampler;
	}
	else
	{
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	}
	imageInfo.imageView = m_image;


	Vulkan::Initializers::WriteDescriptorSetParameters createInfo
	{
		.dstSet = a_descSet,
		.descriptorType = (m_usage == UseMode::Input) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		.dstBinding = m_bindingPoint,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.pImageInfo = &imageInfo,
		.pBufferInfo = nullptr,
		.pTexelBufferView = nullptr
	};
	VkWriteDescriptorSet descSetInfo = Vulkan::Initializers::writeDescriptorSet(createInfo);
	vkUpdateDescriptorSets(m_vkCtxt.m_logicalDevice, 1, &descSetInfo, 0, nullptr);
}

void VE_UniformSampler::operator = (const VkImageView a_other)
{
	m_image = a_other;
	cleanup();
	if (m_usage == UseMode::Input)
	{
		VkSamplerCreateInfo samplerCreateInfo = Vulkan::Initializers::samplerCreateInfo();
		vkCreateSampler(m_vkCtxt.m_logicalDevice, &samplerCreateInfo, nullptr, &m_sampler);
	}
}

void VE_UniformSampler::setUsage(const UseMode a_mode)
{
	VE_Uniform::setUsage(a_mode);
	// nothing to do yet
}