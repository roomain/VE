template<typename Type>
class TPreviousCurrent
{
private:
    Type m_previousValue;
    Type m_currentValue;

public:
    TPreviousCurrent() = delete;
    TPreviousCurrent(Type&& a_default)noexcept : m_previousValue{a_default}, m_currentValue{m_previousValue}{}
    TPreviousCurrent(Type&& a_previous, Type&& a_current)noexcept : m_previousValue{a_previous}, m_currentValue{a_current}{}
    [[nodiscard]] Type previous()const noexcept {return m_previousValue;}
    [[nodiscard]] Type current()const noexcept {return m_currentValue;}
    [[nodiscard]] operator Type()const noexcept {return m_currentValue;}

    Type operator = (const Type& a_newValue)
    {
        m_previousValue = m_currentValue;
        return m_currentValue = a_newValue;
    }

    Type operator == (const Type& a_value)
    {
        return m_currentValue == a_value;
    }
};