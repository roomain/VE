#pragma once
/***********************************************
* @headerfile VE_Camera.h
* @date 08 / 01 / 2025
* @author Roomain
************************************************/
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

/*@brief rendering camera*/
class VE_Camera
{
private:
	glm::dmat4 m_projection;	/*!< projection matrix*/
	glm::dmat4 m_transform;     /*!< transformation matrix*/

public:

    inline glm::dmat4 matrix()const noexcept { return m_transform; }
    inline glm::dmat4 projection()const noexcept { return m_projection; }

#pragma region Projection
    // todo
#pragma endregion

#pragma region Positioning
    // look at from current position
    inline void lookAt(const glm::dvec3& a_target, const glm::dvec3& a_upVector)
    {
        m_transform = glm::lookAt(position(), a_target, a_upVector);
    }

    inline void lookAt(const glm::dvec3& a_position, const glm::dvec3& a_target, const glm::dvec3& a_upVector)
    {
        m_transform = glm::lookAt(a_position, a_target, a_upVector);
    }

    inline void setPosition(const glm::dvec3& a_position)
    {
        m_transform[3] = glm::dvec4(a_position, m_transform[3][3]);
    }

    inline void setRotation(const glm::dvec3& a_axisAngles)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(scale, glm::dquat(a_axisAngles), translation, skew, perspective);
    }

    inline void setScale(const glm::dvec3& a_scale)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(a_scale, rotation, translation, skew, perspective);
    }

    inline void setQuaternion(const glm::dquat& a_quaternion)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(scale, a_quaternion, translation, skew, perspective);
    }

    [[nodiscard]] inline glm::dvec3 position()const
    {
        return glm::xyz(m_transform[3]) * m_transform[3].w;
    }

    [[nodiscard]] inline glm::dvec3 rotation()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return glm::eulerAngles(rotation);
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

    inline void rotateAround(const double& a_angle, const glm::dvec3& a_axis, const glm::dvec3& a_target)
    {
        glm::dvec3 offset = a_target - position();
        m_transform = glm::translate(m_transform, offset);
        m_transform = glm::rotate(m_transform, a_angle, a_axis);
        m_transform = glm::translate(m_transform, -offset);
    }
#pragma endregion
};