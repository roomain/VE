#pragma once
/***********************************************
* @headerfile VE_UniformBuffer.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_Uniform.h"
#include "memory/VulkanBuffer.h"
#include "memory/VE_Memory.h"

/*@brief uniform Buffer use to encapsulate VarType for shader usage*/
template<typename VarType>
class VE_UniformBuffer : public VE_Uniform, public VulkanObject<VE_DeviceContext>
{
private:
    VarType m_uniformVariable;     /*!< uniform variable for host usage*/
    VulkanBuffer m_uniformBuffer;  /*!< uniform buffer for shader usage*/

	explicit VE_UniformBuffer(const uint32_t a_bindingPoint, const VE_DeviceContext& a_ctxt) : VE_Uniform(a_bindingPoint), VulkanObject<VE_DeviceContext>{ a_ctxt }
    {
        //
    }

public:
    VE_UniformBuffer() = delete;

	~VE_UniformBuffer()final { cleanup(); }
    inline VarType& data() { return m_uniformVariable; }

    VarType& operator = (const VarType& a_other)
    {
        m_uniformVariable = a_other;
        return m_uniformVariable;
    }
    
    VarType& operator = (VarType&& a_other)noexcept
    {
        m_uniformVariable = a_other;
        return m_uniformVariable;
    }
    
    void initialize() final
    {
        cleanup(); 
		allocateStagingBuffer(m_vkCtxt, sizeof(VarType));
    }

    void cleanup() final
    {
        if (m_uniformBuffer != VK_NULL_HANDLE)
        {
			releaseBuffer(m_vkCtxt, m_uniformBuffer);
        }
    }

    [[nodiscard]] constexpr bool isValid()const noexcept final { return m_uniformBuffer != VK_NULL_HANDLE; }

    void updateShaderVariable(const VkDescriptorSet a_descSet) final
    {
        // write data to vulkan buffer
		write(m_vkCtxt, m_uniformBuffer, &m_uniformVariable);        

        VkDescriptorBufferInfo bufInfo{
            .buffer = m_uniformBuffer;
            .offset = 0;
            .range = sizeof(VarType);
        };

        Vulkan::Initializers::WriteDescriptorSetParameters createInfo
        {
            .dstSet = a_descSet,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .dstBinding = m_bindingPoint,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .pImageInfo = nullptr,
            .pBufferInfo = &bufInfo,
            .pTexelBufferView = nullptr
        };
        VkWriteDescriptorSet descSetInfo = Vulkan::Initializers::writeDescriptorSet(createInfo);
        vkUpdateDescriptorSets(device, 1, &descSetInfo, 0, nullptr);
    }

    void setUsage(const UseMode a_mode) final
    {
        VE_Uniform::setUsage(a_mode);
        // todo
    }
};