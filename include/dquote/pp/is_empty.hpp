#pragma once

#include <dquote/pp/cat.hpp>

#if !defined(DQUOTE_PP_EMPTY)
# define DQUOTE_PP_IS_EMPTY(...) DQUOTE_PP_IS_EMPTY_II(DQUOTE_PP_CAT(DQUOTE_PP_IS_EMPTY_I ## __VA_ARGS__,), 0)
#endif

#if !defined(DQUOTE_PP_IS_EMPTY_I)
# define DQUOTE_PP_IS_EMPTY_I _, 1
#endif

#if !defined(DQUOTE_PP_IS_EMPTY_II)
# define DQUOTE_PP_IS_EMPTY_II(a, b, ...) DQUOTE_PP_CAT(DQUOTE_PP_IS_EMPTY_III(a, b, __VA_ARGS__),)
#endif

#if !defined(DQUOTE_PP_IS_EMPTY_III)
# define DQUOTE_PP_IS_EMPTY_III(a, b, ...) b
#endif
