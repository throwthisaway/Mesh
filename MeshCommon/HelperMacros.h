#ifndef HELPERMACROS_H_
#define HELPERMACROS_H_
//#define DEL(obj) if (obj) { delete obj; obj = NULL; };
#define DEL(obj) delete obj; obj = NULL;
//#define DEL_ARRAY(obj) if (obj) { delete [] obj; obj = NULL; };
#define DEL_ARRAY(obj) delete [] obj; obj = NULL;
#define SMOOTHING(x) x > 0
#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])
#define R(color) (int)(color & 0xff)/255.f
#define G(color) ((int)(color>>8) & 0xff)/255.f
#define B(color) ((int)(color>>16) & 0xff)/255.f
// Fourth vector component ignored by lighting 
#define W1 1.f
// View position is infinite
#define W0 .0f
#define OPAQUE_COMPONENT(val) val <= 0.f
// Debug helpers
#ifdef _DEBUG
#define _ASSERT_EXPR_A(expr, msg) \
        (void) ((!!(expr)) || \
                (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, msg)) || \
                (_CrtDbgBreak(), 0))
#define _ASSERT_GL_ERROR(expr) {\
    GLint error = expr;\
    _ASSERT_EXPR_A(!error, (const char*)gluErrorString(error));\
    }
#define _LOG_GL_ERROR(expr) \
     { GLint _error = expr;\
     if (_error)\
         Log::CLog::Write("glGetError: %s\n", gluErrorString(_error));\
     }
#else
#define _ASSERT_EXPR_A(expr, msg)
#define _ASSERT_GL_ERROR(expr)
#define _LOG_GL_ERROR(expr)
#endif
#else
//#define _ASSERT_EXPR_A(expr, msg)
//#define _ASSERT_GL_ERROR(expr)
//#define _LOG_GL_ERROR(expr)
#endif
