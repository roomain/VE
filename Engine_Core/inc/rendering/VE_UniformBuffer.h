#pragma once
/***********************************************
* @headerfile VE_UniformBuffer.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_Uniform.h"

/*@brief uniform Buffer use to encapsulate VarType for shader usage*/
template<typename VarType>
class VE_UniformBuffer : public VE_Uniform
{
private:
    VarType m_uniformVariable;                  /*!< uniform variable for host usage*/
    VkBuffer m_uniformBuffer = VK_NULL_HANDLE;  /*!< uniform buffer for shader usage*/

public:
    inline VarType& data();
    VarType& operator = (const VarType& a_other);
    VarType& operator = (VarType&& a_other)noexcept;
    [[nodiscard]] constexpr bool isValid()const noexcept { return m_uniformBuffer != VK_NULL_HANDLE; }
    void updateShaderVariable() final
    {
        /*WriteDescriptorSetParameters createInfo
        {
            .dstSet;
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            .dstBinding = m_bindingPoint;
            .dstArrayElement = 0;
            .descriptorCount = 1;
            .pImageInfo = nullptr;
            .pBufferInfo = nullptr;
            .pTexelBufferView = nullptr;
        };
        VkDescriptorBufferInfo bufferInfo = Vulkan::Initializers::writeDescriptorSet(createInfo);
        vkUpdateDescriptorSets(device, 1, &bufferInfo, 0, nullptr);*/
    }
};