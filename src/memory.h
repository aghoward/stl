#pragma once

#include <stdlib.h>

#include <type_traits.h>
#include <utility.h>

void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);

namespace std
{
    template <typename T>
    T* addressof(T& arg)
    {
        return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(arg)));
    }

    template <typename T>
    class unique_ptr
    {
        private:
            T* _data;
        
        public:
            unique_ptr()
                : _data(nullptr) {}
            
            unique_ptr(T* data)
                : _data(data) {}

            unique_ptr(const unique_ptr&) = delete;
            unique_ptr& operator=(const unique_ptr&) = delete;

            template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
            unique_ptr(unique_ptr<U>&& other)
            {
                _data = other.get();
                other.clear();
            }

            template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
            unique_ptr& operator=(unique_ptr<U>&& other)
            {
                _data = other.get();
                other.clear();
                return *this;
            }

            ~unique_ptr()
            {
                if (_data != nullptr)
                    delete _data;
                _data = nullptr;
            }

            std::add_lvalue_reference_t<T> operator*() const
            {
                return *_data;
            }

            decltype(_data) operator->() const
            {
                return _data;
            }

            decltype(_data) get() const
            {
                return _data;
            }

            void clear()
            {
                _data = nullptr;
            }
    };

    template <typename T, typename ... TArgs>
    unique_ptr<T> make_unique(TArgs&& ... args)
    {
        return unique_ptr<T>(new T(std::forward<TArgs>(args)...));
    }
}
