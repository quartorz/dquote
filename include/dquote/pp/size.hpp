#pragma once

#include <dquote/pp/cat.hpp>
#include <dquote/pp/is_empty.hpp>

#if !defined(DQUOTE_PP_SIZE)
# define DQUOTE_PP_SIZE(...) DQUOTE_PP_CAT(DQUOTE_PP_SIZE_I(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1),)
#endif

#if !defined(DQUOTE_PP_SIZE_I)
# define DQUOTE_PP_SIZE_I(e1, e2, e3, e4, e5, e6, e7, e8, e9, size, ...) DQUOTE_PP_CAT(DQUOTE_PP_SIZE_II, DQUOTE_PP_IS_EMPTY(e1)) (0, size)
#endif

#if !defined(DQUOTE_PP_SIZE_II1)
# define DQUOTE_PP_SIZE_II1(a, b) a
#endif

#if !defined(DQUOTE_PP_SIZE_II0)
# define DQUOTE_PP_SIZE_II0(a, b) b
#endif
