#pragma once
/***********************************************
* @headerfile VE_UniformSampler.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_Uniform.h"

class VE_UniformSampler : public VE_Uniform
{
private:
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;

public:
    [[nodiscard]] constexpr bool isValid()const noexcept { return m_imageView != VK_NULL_HANDLE; }
    void updateShaderVariable() final;
};
