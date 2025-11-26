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
    VkImageView m_image;                    /*!< image view from outside*/
    VkSampler m_sampler = VK_NULL_HANDLE;

    explicit VE_UniformSampler(const uint32_t a_bindingPoint, const VE_DeviceContext& a_ctxt);

public:
    VE_UniformSampler() = delete;
    ~VE_UniformSampler() final;
    [[nodiscard]] constexpr bool isValid()const noexcept final { return m_image != VK_NULL_HANDLE; }
    void initialize()final;
    void cleanup()final;
    void updateShaderVariable(const VkDescriptorSet a_descSet) final;
    void operator = (const VkImageView a_other);
	VkImageView imageview()const noexcept { return m_image; }
    void setUsage(const UseMode a_mode) final;
};
