#pragma once
/***********************************************
* @headerfile VE_Transformation.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class VE_Transformation
{
private:
    glm::dmat4 m_transform;     /*!<transformation*/

    explicit VE_Transformation(glm::dmat4&& a_transform)noexcept : m_transform{ a_transform } {}

public:
    VE_Transformation() : m_transform{ glm::identity< glm::dmat4>() }{}
    explicit VE_Transformation(const VE_Transformation& a_other) = default;
    VE_Transformation(VE_Transformation&& a_other) noexcept : m_transform{ a_other.m_transform } {}
   
    /////////////////////////////////////////////
    // EDITION FUNCTION
    /////////////////////////////////////////////

    void setPosition(const glm::dvec3& a_position)
    {
        m_transform[3] = glm::dvec4(a_position, m_transform[3][3]);
    }

    void setRotation(const glm::dvec3& a_axisAngles)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(scale, glm::dquat(a_axisAngles), translation, skew, perspective);
    }

    void setScale(const glm::dvec3& a_scale)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(a_scale, rotation, translation, skew, perspective);
    }

    void setQuaternion(const glm::dquat& a_quaternion)const
    {
        //
    }

    [[nodiscard]] glm::dvec3 position()const
    {
        return glm::xyz(m_transform[3]) * m_transform[3].w;
    }

    [[nodiscard]] glm::dvec3 rotation()const
    {
        //
    }

    [[nodiscard]] glm::dvec3 scale()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return scale;
    }

    [[nodiscard]] glm::dquat quaternion()const
    {        
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return rotation;
    }

    inline void rotateAround(const double& a_angle, const glm::dvec3& a_axis, const glm::dvec3& a_position)
    {
        glm::dvec3 offset = a_position - position();
        m_transform = glm::translate(m_transform , offset);
        m_transform = glm::rotate(m_transform,  a_angle, a_axis);
        m_transform = glm::translate(m_transform, -offset);
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
