#pragma once
/***********************************************
* @headerfile VE_Transformation.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class VE_Transformation
{
private:
    glm::dmat4 m_transform;     /*!< local transformation*/

    VE_Transformation(glm::dmat4&& a_transform)noexcept : m_transform{ m_transform } {}

public:
    VE_Transformation() : m_transform{ glm::identity< glm::dmat4>() }{}
    explicit VE_Transformation(const VE_Transformation& a_other) = default;
    VE_Transformation(VE_Transformation&& a_other) noexcept : m_transform{ a_other.m_transform } {}
    ~VE_Transformation() = default;

    /////////////////////////////////////////////
    // EDITION FUNCTION
    /////////////////////////////////////////////

    void setPosition(const glm::dvec3& a_position)
    {
        m_transform[3] = glm::dvec4(a_position, m_transform[3][3]);
    }

    void setRotation(const glm::dvec3& a_axisAngles)
    {
        //
    }

    void setScale(const glm::dvec3& a_scale)
    {
        //
    }

    void setQuaternion(const glm::dquat& a_quaternion)const
    {
        //
    }

    [[nodiscard]] glm::dvec3 position()const
    {
        return glm::xyz(m_transform[3]);
    }

    [[nodiscard]] glm::dvec3 rotation()const
    {
        //
    }

    [[nodiscard]] glm::dvec3 scale()const
    {
        //
    }

    [[nodiscard]] glm::dquat quaternion()const
    {
        //
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] inline VE_Transformation& operator = (const VE_Transformation& a_other) = default;
    [[nodiscard]] inline VE_Transformation& operator = (VE_Transformation&& a_other)noexcept = default;

    [[nodiscard]] friend inline VE_Transformation operator + (const VE_Transformation& a_transform, const VE_Transformation& a_other)
    {
        return VE_Transformation(a_transform.m_transform + a_other.m_transform);
    }

    [[nodiscard]] friend inline VE_Transformation operator - (const VE_Transformation& a_transform, const VE_Transformation& a_other)
    {
        return VE_Transformation(a_transform.m_transform - a_other.m_transform);
    }

    [[nodiscard]] friend inline VE_Transformation operator * (const VE_Transformation& a_transform, const VE_Transformation& a_other)
    {
        return VE_Transformation(a_transform.m_transform * a_other.m_transform);
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    [[nodiscard]] friend inline VE_Transformation operator * (const VE_Transformation& a_transform, const ArithmeticType a_other)
    {
        return VE_Transformation(a_transform.m_transform * a_other);
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    [[nodiscard]] friend inline VE_Transformation operator / (const VE_Transformation& a_transform, const ArithmeticType a_other)
    {
        return VE_Transformation(a_transform.m_transform / a_other);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void operator += (const VE_Transformation& a_other)
    {
        m_transform += a_other.m_transform;
    }

    inline void operator -= (const VE_Transformation& a_other)
    {
        m_transform -= a_other.m_transform;
    }

    inline void operator *= (const VE_Transformation& a_other)
    {
        m_transform *= a_other.m_transform;
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    inline void operator *= (const ArithmeticType a_other)
    {
        m_transform += a_other;
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    inline void operator /= (const ArithmeticType a_other)
    {
        m_transform /= a_other;
    }
};
