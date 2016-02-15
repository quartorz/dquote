#pragma once

#include <dquote/pp/size.hpp>
#include <dquote/pp/cat.hpp>

#if !defined(DQUOTE_PP_OVERLOAD)
# define DQUOTE_PP_OVERLOAD(name, ...) DQUOTE_PP_CAT(DQUOTE_PP_CAT(name, DQUOTE_PP_SIZE(__VA_ARGS__)) (__VA_ARGS__),)
#endif
