#pragma once
/***********************************************
* @headerfile VE_UniformSampler.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_Uniform.h"
#include "utils/VulkanContext.h"

class VE_UniformSampler : public VE_Uniform, public VulkanObject<VE_DeviceContext>
{
private:
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;

    explicit VE_UniformSampler(const uint32_t a_bindingPoint, const VE_DeviceContext& a_ctxt);

public:
    ~VE_UniformSampler() final;
    [[nodiscard]] constexpr bool isValid()const noexcept { return m_imageView != VK_NULL_HANDLE; }
    void initialize()final;
    void cleanup()final;
    void updateShaderVariable() final;
};
