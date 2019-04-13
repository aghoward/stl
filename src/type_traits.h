#pragma once

namespace std {
    struct true_type
    {
        static constexpr bool value = true;
    };

    struct false_type
    {
        static constexpr bool value = false;
    };

    template <typename T, typename V>
    struct is_same : false_type {};

    template <typename T>
    struct is_same<T, T> : true_type {};

    template <typename T, typename V>
    static constexpr bool is_same_v = is_same<T, V>::value;

    template <typename T>
    constexpr T&& declval();

    template <typename T, typename U>
    struct is_convertible
    {
        template <typename V>
        static constexpr true_type matches(U value);

        template <typename V>
        static constexpr false_type matches(...);

        static constexpr bool value = decltype(matches<T>(declval<T>()))::value;
    };

    template <typename T, typename U>
    static constexpr bool is_convertible_v = is_convertible<T, U>::value;

    template <bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> { typedef T type; };

    template <bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template <bool B, typename TTrue, typename TFalse>
    struct conditional
    {
        using type = TTrue;
    };

    template <typename TTrue, typename TFalse>
    struct conditional<false, TTrue, TFalse>
    {
        using type = TFalse;
    };

    template <bool Bool, typename TTrue, typename TFalse>
    using conditional_t = typename conditional<Bool, TTrue, TFalse>::type;

    template <bool Bool, typename T, T TrueValue, T FalseValue>
    struct conditional_value
    {
        static constexpr T value = TrueValue;
    };

    template <typename T, T TrueValue,T FalseValue>
    struct conditional_value<false, T, TrueValue, FalseValue>
    {
        static constexpr T value = FalseValue;
    };

    template <bool Bool, typename T, T TrueValue, T FalseValue>
    static constexpr T conditional_value_v = conditional_value<Bool, T, TrueValue, FalseValue>::value;

    template<typename...>
    struct disjunction : false_type { };

    template<typename Bool1>
    struct disjunction<Bool1> : Bool1 { };

    template<typename Bool1, typename ... BoolN>
    struct disjunction<Bool1, BoolN...> 
        : conditional_t<bool(Bool1::value), Bool1, disjunction<BoolN...>>  { };

    template <typename ... BoolN>
    static constexpr bool disjunction_v = disjunction<BoolN...>::value;

    template <typename T>
    struct remove_reference
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    struct add_lvalue_reference
    {
        using type = T&;
    };

    template <typename T>
    struct add_lvalue_reference<T&>
    {
        using type = T&;
    };

    template <typename T>
    struct add_lvalue_reference<T&&>
    {
        using type = T&&;
    };

    template <typename T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    template <typename T>
    struct remove_cv
    {
        using type = T;
    };

    template <typename T>
    struct remove_cv<const T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };

    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <typename T>
    using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

    template <typename T>
    struct add_const
    {
        using type = const T;
    };

    template <typename T>
    struct add_const<const T>
    {
        using type = const T;
    };

    template <typename T>
    struct add_const<T*>
    {
        using type = const T*;
    };

    template <typename T>
    using add_const_t = typename add_const<T>::type;

    template <typename F, typename ... TArgs>
    struct invoke_result
    {
        using type = decltype(declval<F>()(declval<TArgs>()...));
    };

    template <typename F, typename ... TArgs>
    using invoke_result_t = typename invoke_result<F, TArgs...>::type;

    namespace detail {
        template <typename T>
        struct _is_integral : false_type {};

        template <>
        struct _is_integral<short> : true_type {};

        template <>
        struct _is_integral<int> : true_type {};
        
        template <>
        struct _is_integral<long> : true_type {};

        template <>
        struct _is_integral<long long> : true_type {};

        template <>
        struct _is_integral<unsigned short> : true_type {};

        template <>
        struct _is_integral<unsigned int> : true_type {};
        
        template <>
        struct _is_integral<unsigned long> : true_type {};

        template <>
        struct _is_integral<unsigned long long> : true_type {};
    }

    template <typename T>
    struct is_integral : detail::_is_integral<remove_cvref_t<T>> {};

    template <typename T>
    static constexpr bool is_integral_v = is_integral<T>::value;

    template <typename ...>
    using void_t = void;

    namespace detail {
        template <typename T, typename Enabler = void, typename ... TArgs>
        struct is_constructible : false_type {};

        template <typename T, typename ... TArgs>
        struct is_constructible<T, void_t<decltype(T{declval<TArgs>()...})>, TArgs...> : true_type {};
    }

    template <typename T, typename ... TArgs>
    using is_constructible = detail::is_constructible<T, void, TArgs...>;

    template <typename T, typename ... TArgs>
    static constexpr bool is_constructible_v = is_constructible<T, TArgs...>::value;

    template <typename T>
    using is_copy_constructible = is_constructible<T, add_const_t<add_lvalue_reference_t<T>>>;

    template <typename T>
    static constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
}
