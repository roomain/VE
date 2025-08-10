#pragma once
/***********************************************
* @headerfile VE_Capabilities.h
* @date 09 / 07 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <vulkan/vulkan.hpp>


class VE_CapabilitiesVisitor;

/*@brief base class for vulkan capabilities*/
class VE_Capabilities
{
protected:
    std::vector<VkExtensionProperties> m_extensions;   /*!< extension properties*/
    std::vector<VkLayerProperties> m_layers;            /*!< layer properties*/

public:
    VE_Capabilities() = default;
    virtual ~VE_Capabilities() = default;

    virtual void visit(VE_CapabilitiesVisitor& a_visitor)const;

    using Extension_const_iterator = std::vector<VkExtensionProperties>::const_iterator;
    [[nodiscard]] Extension_const_iterator extensionBegin()const noexcept;
    [[nodiscard]] Extension_const_iterator extensionEnd()const noexcept;

    using Layer_const_iterator = std::vector<VkLayerProperties>::const_iterator;
    [[nodiscard]] Layer_const_iterator layerBegin()const noexcept;
    [[nodiscard]] Layer_const_iterator layerEnd()const noexcept;

    [[nodiscard]] bool checkExtensions(const std::vector<std::string>& a_extensions)const noexcept;
    [[nodiscard]] bool checkLayers(const std::vector<std::string>& a_layers)const noexcept;
};

