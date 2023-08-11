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

inline unsigned int type_id_seq = 0;
template <typename T>
inline const unsigned int type_id = type_id_seq++;

class fast_any final
{
public:
    fast_any( const fast_any& ) = delete;
    fast_any& operator=( const fast_any& ) = delete;

    inline fast_any();
    inline fast_any( fast_any&& );
    inline ~fast_any();

    inline bool has_value() const;

    template <typename T>
    inline T* get() const;

    template <typename T>
    inline void emplace( const T& newValue );
    inline void emplace( const fast_any& fromSignal );

    template <typename T>
    inline void move( T&& newValue );
    inline void move( fast_any& fromSignal );

    inline void reset();

    inline unsigned int type() const;

private:
    struct _value_holder
    {
        _value_holder( const _value_holder& ) = delete;
        _value_holder& operator=( const _value_holder& ) = delete;

        inline _value_holder() = default;
        virtual inline ~_value_holder() = default;

        virtual inline _value_holder* get_copy() const = 0;
        virtual inline void emplace( _value_holder* valueHolder ) = 0;
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

        virtual inline _value_holder* get_copy() const override
        {
            return new _value( value );
        }

        virtual inline void emplace( _value_holder* valueHolder ) override
        {
            value = ( (_value<T>*)valueHolder )->value;
        }

        const unsigned int type;
        T value;
    };

    _value_holder* _value_holder = nullptr;
    bool _has_value = false;
};

inline fast_any::fast_any() = default;

inline fast_any::fast_any( fast_any&& )
{
}

inline fast_any::~fast_any()
{
    delete _value_holder;
}

inline bool fast_any::has_value() const
{
    return _has_value;
}

template <typename T>
inline T* fast_any::get() const
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
inline void fast_any::emplace( const T& newValue )
{
    if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == type_id<T> )
    {
        ( (_value<T>*)_value_holder )->value = newValue;
    }
    else
    {
        delete _value_holder;
        _value_holder = new _value<T>( newValue );
    }
    _has_value = true;
}

inline void fast_any::emplace( const fast_any& fromSignal )
{
    if ( fromSignal._has_value )
    {
        if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == ( (_value<nullptr_t>*)fromSignal._value_holder )->type )
        {
            _value_holder->emplace( fromSignal._value_holder );
        }
        else
        {
            delete _value_holder;
            _value_holder = fromSignal._value_holder->get_copy();
        }

        _has_value = true;
    }
}

template <typename T>
inline void fast_any::move( T&& newValue )
{
    if ( _value_holder && ( (_value<nullptr_t>*)_value_holder )->type == type_id<T> )
    {
        ( (_value<T>*)_value_holder )->value = std::move( newValue );
    }
    else
    {
        delete _value_holder;
        _value_holder = new _value<T>( std::move( newValue ) );
    }
    _has_value = true;
}

inline void fast_any::move( fast_any& fromSignal )
{
    if ( fromSignal._has_value )
    {
        std::swap( fromSignal._value_holder, _value_holder );
        std::swap( fromSignal._has_value, _has_value );
    }
}

inline void fast_any::reset()
{
    _has_value = false;
}

inline unsigned int fast_any::type() const
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
