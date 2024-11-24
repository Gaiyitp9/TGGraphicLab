/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Exception/OpenGLException.h"
#include "DynamicGles.h"

namespace TG
{
    OpenGLException::OpenGLException(std::string_view description)
    {
        GLenum error;
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
                case GL_STACK_UNDERFLOW:
                    errorMsg = "An attempt has been made to perform an operation that "
                               "would cause an internal stack to underflow.";
                    break;
                case GL_STACK_OVERFLOW:
                    errorMsg = "An attempt has been made to perform an operation that "
                               "would cause an internal stack to overflow.";
                    break;
                default:
                    errorMsg = "Unknown error.";
                    break;
            }

            m_whatBuffer += std::format("Exception type: OpenGL Exception\n"
                                     "Error: {:#04x}\nError Message: {}\n"
                                     "[OpenGL] {}\n"
                                     "{}\n", error, errorMsg, description, m_stackTrace);
        }
    }

    char const* OpenGLException::what() const
    {
        return m_whatBuffer.c_str();
    }
}
