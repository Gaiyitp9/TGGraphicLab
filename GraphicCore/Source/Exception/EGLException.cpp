/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Exception/EGLException.h"
#include "DynamicEgl.h"

namespace TG
{
    EGLException::EGLException(std::string_view description)
    {
        EGLint error = eglGetError();
        char const* errorMsg = nullptr;
        switch (error)
        {
            case EGL_NOT_INITIALIZED:
                errorMsg = "EGL is not initialized, or could not be initialized, "
                           "for the specified EGL display connection.";
                break;
            case EGL_BAD_ACCESS:
                errorMsg = "EGL cannot access a requested resource "
                           "(for example a context is bound in another thread).";
                break;
            case EGL_BAD_ALLOC:
                errorMsg = "EGL failed to allocate resources for the requested operation.";
                break;
            case EGL_BAD_ATTRIBUTE:
                errorMsg = "An unrecognized attribute or attribute value was passed in the attribute list.";
                break;
            case EGL_BAD_CONTEXT:
                errorMsg = "An EGLContext argument does not name a valid EGL rendering context.";
                break;
            case EGL_BAD_CONFIG:
                errorMsg = "An EGLConfig argument does not name a valid EGL frame buffer configuration.";
                break;
            case EGL_BAD_CURRENT_SURFACE:
                errorMsg = "The current surface of the calling thread is a window, pixel buffer or pixmap that "
                           "is no longer valid.";
                break;
            case EGL_BAD_DISPLAY:
                errorMsg = "An EGLDisplay argument does not name a valid EGL display connection.";
                break;
            case EGL_BAD_SURFACE:
                errorMsg = "An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) "
                           "configured for GL rendering.";
                break;
            case EGL_BAD_MATCH:
                errorMsg = "Arguments are inconsistent "
                           "(for example, a valid context requires buffers not supplied by a valid surface).";
                break;
            case EGL_BAD_PARAMETER:
                errorMsg = "One or more argument values are invalid.";
                break;
            case EGL_BAD_NATIVE_PIXMAP:
                errorMsg = "A NativePixmapType argument does not refer to a valid native pixmap.";
                break;
            case EGL_BAD_NATIVE_WINDOW:
                errorMsg = "A NativeWindowType argument does not refer to a valid native window.";
                break;
            case EGL_CONTEXT_LOST:
                errorMsg = "A power management event has occurred. The application must destroy all contexts and "
                           "reinitialise OpenGL ES state and objects to continue rendering.";
                break;
            default:
                errorMsg = "Unknown error.";
                break;
        }

        m_whatBuffer = std::format("Exception type: EGL Exception\n"
                                 "Error: {:#04x}\nError Message: {}\n"
                                 "[EGL] {}\n"
                                 "{}\n", error, errorMsg, description, m_stackTrace);
    }

    char const* EGLException::what() const
    {
        return m_whatBuffer.c_str();
    }
}
