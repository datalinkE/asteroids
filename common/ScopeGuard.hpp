#pragma once

#include <functional>

namespace Util
{
    class ScopeGuard
    {
    public:
        ScopeGuard(std::function<void(void)> action)
            : m_action(action)
        {
        }

        ~ScopeGuard()
        {
            m_action();
        }
    private:
        std::function<void(void)> m_action;
    };
}

#define CONCAT_WITHOUT_EXPANDING(a, b)    a ## b

#define CONCAT(a, b) CONCAT_WITHOUT_EXPANDING(a, b) // expand than concat

#define ON_BLOCK_EXIT(unary_function, arg) Util::ScopeGuard CONCAT(scopeGuard, __LINE__)(std::bind(unary_function, arg))
