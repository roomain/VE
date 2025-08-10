#pragma once
#include <variant>
#include <string>
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <functional>
#include <format>
#include <memory>
#include "reflectionTraits.h"
#include "ReflectionException.h"
#include "ReflectionManager.h"
#include "ReflectionGlobals.h"


#pragma warning(push)
#pragma warning( disable : 4251 )

using VariantValue = std::variant<int, unsigned int, double, std::string, ReflectionClassPtr>;
using VariantMember = std::variant<int, unsigned int, double, std::string, std::vector<VariantValue>, ReflectionClassPtr>;
using ToIntFunction = std::function<int(const std::string&)>;

// limitation: can't have a matrix data

class REFLECTION_LIB ReflectionValue
{
    friend class ReflectionJsonParser;

private:
    VariantMember m_variantData;

    // add functions for specific enum_cast
    static inline std::unordered_map<std::string, ToIntFunction> m_enumCast;

    void affect(const bool a_value);
    void affect(const int a_value);
    void affect(const unsigned int a_value);
    void affect(const double& a_value);
    void affect(const std::string& a_value);
    void affect(const ReflectionClassPtr& a_value);
    void affect(std::vector<VariantValue>&& a_value);

    template<typename Type>
    struct SimpleVisitor
    {
        Type& data;// data to fill
        void operator()(const bool a_value)
        {
            if constexpr (std::is_assignable_v<Type&, bool> && !std::is_same_v<Type, std::string>)
            {
                data = a_value;
            }
            else
            {
                // log error
            }
        }

        void operator()(const int a_value)
        {
            if constexpr (std::is_assignable_v<Type&, int> && !std::is_same_v<Type, std::string>)
            {
                data = static_cast<Type>(a_value);
            }
            else
            {
                // log error
            }
        }

        void operator()(const unsigned int a_value)
        {
            if constexpr (std::is_assignable_v<Type&, unsigned int> && !std::is_same_v<Type, std::string>)
            {
                data = static_cast<Type>(a_value);
            }
            else
            {
                // log error
            }
        }

        void operator()(const double& a_value)
        {
            if constexpr (std::is_assignable_v<Type&, double> && !std::is_same_v<Type, std::string>)
            {
                data = static_cast<Type>(a_value);
            }
            else
            {
                // log error
            }
        }

        void operator()(const std::string& a_value)
        {
            if constexpr (std::is_enum_v<Type>)
            {
                if (const auto iter = m_enumCast.find(typeid(Type).name()); iter != m_enumCast.cend())
                {
                    data = static_cast<Type>(iter->second(a_value));
                }
                else
                {
                    // log error
                }
            }
            else if constexpr (std::is_assignable_v<Type&, std::string>)
            {
                data = a_value;
            }
            else
            {
                // log error
            }
        }

        void operator()(const std::vector<VariantValue>&)
        {
            // log error
        }

        void operator()(const ReflectionClassPtr& a_value)
        {
            if constexpr (Is_reflective<Type>::value)
            {
                // set override
                ReflectionManager::instance().overrideClass(Type::getClassname(), a_value);
                Type::deserialize(&data);
                // release Overrride
                ReflectionManager::instance().resetOverride(Type::getClassname());
            }
            else
            {
                // log error
            }
        }

    };

    template<typename Type>
    struct VectorVisitor
    {
        std::vector<Type>& data;// data to fill
        void operator()(const bool)
        {
            // log error
        }

        void operator()(const int)
        {
            // log error
        }

        void operator()(const unsigned int)
        {
            // log error
        }

        void operator()(const double&)
        {
            // log error
        }

        void operator()(const std::string&)
        {
            // log error
        }

        void operator()(const std::vector<VariantValue>& a_value)
        {
            for (const VariantValue& value : a_value)
            {
                Type tmp;
                SimpleVisitor<Type> visitor(tmp);
                std::visit(visitor, value);
                data.emplace_back(std::move(tmp));
            }
        }

        void operator()(const ReflectionClassPtr&)
        {
            // log error
        }
    };


public:
    ReflectionValue() = default;
    virtual ~ReflectionValue() = default;
    template<typename EnumType> //requires std::is_enum_v<EnumType>
    static void registerCast(const ToIntFunction& a_callback)
    {
        m_enumCast.emplace(typeid(EnumType).name(), a_callback);
    }

    template<typename Type>
    void set(std::vector<Type>& a_member)const
    {
        VectorVisitor<Type> visitor{ a_member };
        std::visit(visitor, m_variantData);
    }

    template<typename Type>
    void set(Type& a_member)const
    {
        SimpleVisitor<Type> visitor{ a_member };
        std::visit(visitor, m_variantData);
    }
};

#pragma warning(pop)