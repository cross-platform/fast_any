/******************************************************************************
fast_any - A faster alternative to boost:any / std::any
Copyright (c) 2023, Marcus Tomlinson

BSD 2-Clause License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#pragma once

#include <memory>

namespace fast_any
{

using type_info = unsigned int;
inline type_info type_info_seq = 0;
template <typename T>
inline const type_info type_id = type_info_seq++;

class any final
{
public:
    inline any();
    inline ~any();

    inline any( const any& other );
    inline any( any&& other );
    inline any& operator=( const any& other );
    inline any& operator=( any&& other );

    inline bool has_value() const;

    template <typename T>
    inline T* as() const;

    template <typename T>
    inline void emplace( const T& value );

    template <typename T>
    inline void emplace( T&& value );

    inline void emplace( const any& other );
    inline void emplace( any&& other );

    inline void swap( any& other );

    inline void reset();

    inline const type_info& type() const;

private:
    struct _value_holder
    {
        _value_holder( const _value_holder& ) = delete;
        _value_holder& operator=( const _value_holder& ) = delete;

        inline _value_holder() = default;
        virtual inline ~_value_holder() = default;

        virtual inline _value_holder* clone() const = 0;
        virtual inline void emplace( _value_holder* value_holder ) = 0;
    };

    template <typename T>
    struct _value final : _value_holder
    {
        _value( const _value& ) = delete;
        _value& operator=( const _value& ) = delete;

        explicit inline _value( const T& value )
            : type( type_id<T> )
            , value( value )
        {
        }

        virtual inline _value_holder* clone() const override
        {
            return new _value( value );
        }

        virtual inline void emplace( _value_holder* value_holder ) override
        {
            value = ( (_value<T>*)value_holder )->value;
        }

        const type_info type;
        T value;
    };

    _value_holder* _value_holder = nullptr;
    bool _has_value = false;
};

inline any::any() = default;

inline any::~any()
{
    delete _value_holder;
}

inline any::any( const any& other )
{
    emplace( other );
}

inline any::any( any&& other )
{
    emplace( std::move( other ) );
}

inline any& any::operator=( const any& other )
{
    emplace( other );
    return *this;
}

inline any& any::operator=( any&& other )
{
    emplace( std::move( other ) );
    return *this;
}

inline bool any::has_value() const
{
    return _has_value;
}

template <typename T>
inline T* any::as() const
{
    if ( _has_value && ( (_value<nullptr_t>*)_value_holder )->type == type_id<T> )
    {
        return &( (_value<T>*)_value_holder )->value;
    }
    else
    {
        return nullptr;
    }
}

template <typename T>
inline void any::emplace( const T& value )
{
    if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == type_id<T> )
    {
        ( (_value<T>*)_value_holder )->value = value;
    }
    else
    {
        delete _value_holder;
        _value_holder = new _value<T>( value );
    }
    _has_value = true;
}

template <typename T>
inline void any::emplace( T&& value )
{
    if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == type_id<T> )
    {
        ( (_value<T>*)_value_holder )->value = std::move( value );
    }
    else
    {
        delete _value_holder;
        _value_holder = new _value<T>( std::move( value ) );
    }
    _has_value = true;
}

inline void any::emplace( const any& other )
{
    if ( other._has_value )
    {
        if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == ( (_value<nullptr_t>*)other._value_holder )->type )
        {
            _value_holder->emplace( other._value_holder );
        }
        else
        {
            delete _value_holder;
            _value_holder = other._value_holder->clone();
        }

        _has_value = true;
    }
}

inline void any::emplace( any&& other )
{
    if ( other._has_value )
    {
        _value_holder = std::move( other._value_holder );
        _has_value = std::move( other._has_value );
    }
}

inline void any::swap( any& other )
{
    if ( other._has_value )
    {
        std::swap( other._value_holder, _value_holder );
        std::swap( other._has_value, _has_value );
    }
}

inline void any::reset()
{
    _has_value = false;
}

inline const type_info& any::type() const
{
    if ( _value_holder )
    {
        return ( (_value<nullptr_t>*)_value_holder )->type;
    }
    else
    {
        return type_id<void>;
    }
}

}  // namespace fast_any
