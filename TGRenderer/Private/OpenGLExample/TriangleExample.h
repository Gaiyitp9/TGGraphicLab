/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "glad/gl.h"

namespace TG
{
    class TriangleExample final : public Example
    {
    public:
	    TriangleExample();
        ~TriangleExample() override;

        void Draw() override;
        void DrawUI() override;

    private:
        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};

    	const char* m_vertexShaderSource = R"(
			#version 410 core
			layout(location = 0) in vec3 aPos;
    		layout(location = 1) in vec3 aColor;

    		layout(location = 0) out vec3 vColor;

    		out gl_PerVertex
			{
				vec4 gl_Position;
			};

    		void main()
    		{
    			gl_Position = vec4(aPos, 1.0);
    			vColor = aColor;
    		}
		)";

    	const char* m_fragmentShaderSource = R"(
			#version 410 core
			layout(location = 0) in vec3 fColor;

			layout(location = 0) out vec4 FragColor;

			void main()
			{
			    FragColor = vec4(fColor, 1.0);
			}
		)";
    	GLuint m_vertexShader;
    	GLuint m_fragmentShader;
        GLuint m_pipeline{};
        bool m_wireframe{ false };
    };
}
