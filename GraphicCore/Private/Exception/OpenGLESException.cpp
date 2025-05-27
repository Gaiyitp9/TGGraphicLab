/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Exception/OpenGLESException.h"
#include "DynamicGles.h"
#include <format>
#include <stacktrace>

namespace TG
{
    OpenGLESException::OpenGLESException(std::string_view message) : BaseException(message) {}

    OpenGLESException OpenGLESException::Create(std::string_view message)
    {
        GLenum error;
        std::string whatBuffer;
        while ((error = glGetError()) != GL_NO_ERROR)
        {
            char const* errorMsg = nullptr;
            switch (error)
            {
                case GL_INVALID_ENUM:
                    errorMsg = "An unacceptable value is specified for an enumerated argument.";
                    break;
                case GL_INVALID_VALUE:
                    errorMsg = "A numeric argument is out of range.";
                    break;
                case GL_INVALID_OPERATION:
                    errorMsg = "The specified operation is not allowed in the current state.";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    errorMsg = "The framebuffer object is not complete.";
                    break;
                case GL_OUT_OF_MEMORY:
                    errorMsg = "There is not enough memory left to execute the command.";
                    break;
                default:
                    errorMsg = "Unknown error.";
                    break;
            }

            whatBuffer += std::format("Exception type: OpenGL Exception\n"
                                     "Error: {:#04x}\nError Message: {}\n"
                                     "[OpenGL] {}\n"
                                     "{}\n", error, errorMsg, message, std::stacktrace::current());
        }
        return OpenGLESException(whatBuffer);
    }
}
