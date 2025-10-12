/***********************************************
* @headerfile iterators.h
* @date 07 / 10 / 2024
* @author Roomain
************************************************/

/*@brief define iteration on member of type 'type'*/
#define DEFINE_ITER(type, member) \
[[nodiscard]] constexpr type::iterator begin() { return member.begin(); } \
[[nodiscard]] constexpr type::iterator end() { return member.end(); }

/*@brief define reverse iteration on member of type 'type'*/
#define DEFINE_REV_ITER(type, member) \
[[nodiscard]] constexpr type::reverse_iterator rbegin() { return member.rbegin(); } \
[[nodiscard]] constexpr type::reverse_iterator rend() { return member.rend(); }

/*@brief define const iteration on member of type 'type'*/
#define DEFINE_CONST_ITER(type, member)\
[[nodiscard]] constexpr type::const_iterator cbegin()const { return member.cbegin(); } \
[[nodiscard]] constexpr type::const_iterator cend()const { return member.cend(); }

/*@brief define const reverse iteration on member of type 'type'*/
#define DEFINE_CONST_REV_ITER(type, member)\
[[nodiscard]] constexpr type::const_iterator rcbegin()const { return member.rcbegin(); } \
[[nodiscard]] constexpr type::const_iterator rcend()const { return member.rcend(); }

/*@brief define all mode of iterations on member of type 'type'*/
#define DEFINE_ALL_ITER(type, member)\
DEFINE_ITER(type, member) \
DEFINE_REV_ITER(type, member) \
DEFINE_CONST_ITER(type, member) \
DEFINE_CONST_REV_ITER(type, member)