#pragma once

#if defined _MSC_VER

#define DQUOTE_SELECTANY __declspec(selectany)

#else

#define DQUOTE_SELECTANY __attribute__((selectany))

#endif
