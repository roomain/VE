#pragma once
/***********************************************
* @headerfile VE_Uniform.h
* @date 23 / 08 / 2025
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>
#include "core_globals.h"

/*@brief interface for uniform data*/
class VE_CORE_LIB VE_Uniform
{
public:
    enum class UseMode
    {
        Input,  /*!< shader input*/
        Output  /*!< shader output*/
    };
protected:
    uint32_t m_bindingPoint;            /*!< binding id*/
    UseMode m_usage = UseMode::Input;   /*!< default is input mode*/

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
    virtual void updateShaderVariable(const VkDescriptorSet a_descSet) = 0;
    /*@brief set usage mode*/
    virtual void setUsage(const UseMode a_mode) = 0 { m_usage = a_mode; }
};
