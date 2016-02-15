#pragma once

#if !defined(DQUOTE_PP_CAT)
# define DQUOTE_PP_CAT(a, b) DQUOTE_PP_CAT_I(a, b)
#endif

#if !defined(DQUOTE_PP_CAT_I)
# define DQUOTE_PP_CAT_I(a, b) a ## b
#endif