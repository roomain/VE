#pragma once
/***********************************************
* @headerfile VE_Uniform.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>

/*@brief interface for uniform data*/
class VE_Uniform
{
protected:
    uint32_t m_bindingPoint; /*!< binding id*/

public:
    VE_Uniform() = delete;
    explicit VE_Uniform(const uint32_t a_bindingPoint) : m_bindingPoint{ a_bindingPoint } {}
	virtual ~VE_Uniform() = default;
	/*@brief allocate resource*/
    virtual void initialize() = 0;
	/*@brief release resource*/
    virtual void cleanup() = 0;
    /*@brief check validity*/
    [[nodiscard]] virtual constexpr bool isValid()const noexcept = 0;
    /*@brief update unifor in pipeline*/
    virtual void updateShaderVariable() = 0;
};
