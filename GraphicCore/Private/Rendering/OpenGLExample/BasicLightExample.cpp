/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGLExample/BasicLightExample.h"
#include "Rendering/Color/StandardColors.h"
#include "Rendering/RayTracing/PathTracer.h"
#include "Math/Transform/Transform.hpp"
#include "Diagnostic/Log.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "tiny_gltf.h"

namespace TG::Rendering
{
    struct CameraData
    {
        Math::Matrix4f view;
        Math::Matrix4f projection;
    };

    struct RenderData
    {
        Math::Vector4f lightDirection;
        Math::Vector4f lightColor;
        Math::Vector4f viewPosition;
    };

    BasicLightExample::BasicLightExample(const IDefaultVideoPort& videoPort, const ITimer& timer)
        : m_timer(timer), m_camera(videoPort, timer),
        m_planeMesh(20.0f, 20.0f, 20, 20),
        m_planeProperty{
            Math::Vector3f{ 0.0f, 0.01f, -10.0f },
            SlateGray,
            0.3f, 0.5f, 64.0f },
        m_vertexShader("Assets/Shaders/GLSL/LightingModel/Phong/basic_lighting.vert", ShaderStage::Vertex),
        m_fragmentShader("Assets/Shaders/GLSL/LightingModel/Phong/phong.frag", ShaderStage::Fragment),
        m_wireframeGeometryShader("Assets/Shaders/GLSL/wireframe.geom", ShaderStage::Geometry),
        m_wireframeFragmentShader("Assets/Shaders/GLSL/wireframe.frag", ShaderStage::Fragment)
    {
        LoadRubberToy();
        m_rubberToyProperty = {
            Math::Vector3f{ 0.0f, 1.0f, -14.0f },
            White,
            0.3f, 0.5f, 64.0f
        };

        m_sphereProperties[0] = {
            Math::Vector3f{ -6.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 8.0f
        };
        m_sphereProperties[1] = {
            Math::Vector3f{ -3.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 16.0f
        };
        m_sphereProperties[2] = {
            Math::Vector3f{ 0.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 32.0f
        };
        m_sphereProperties[3] = {
            Math::Vector3f{ 3.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 64.0f
        };
        m_sphereProperties[4] = {
            Math::Vector3f{ 6.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 128.0f
        };

        m_lightDirection = Math::Vector4f{ 1.0f, 1.0f, 1.0f, 0.0f };
        m_lightColor = White;

        glGenBuffers(1, &m_sphereVBO);
        glGenBuffers(1, &m_sphereEBO);
        glGenVertexArrays(1, &m_sphereVAO);

        glBindVertexArray(m_sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
        auto verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.vertices.size());
        auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_sphereMesh.uvs.size());
        auto normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.normals.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_sphereMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_sphereMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize, m_sphereMesh.normals.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_sphereMesh.indices.size()),
            m_sphereMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        glGenBuffers(1, &m_planeVBO);
        glGenBuffers(1, &m_planeEBO);
        glGenVertexArrays(1, &m_planeVAO);

        glBindVertexArray(m_planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
        verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_planeMesh.vertices.size());
        uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_planeMesh.uvs.size());
        normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_planeMesh.normals.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_planeMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_planeMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize, m_planeMesh.normals.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_planeMesh.indices.size()),
            m_planeMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        glGenBuffers(1, &m_cameraUbo);
        glGenBuffers(1, &m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_cameraUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_renderUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(RenderData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenProgramPipelines(1, &m_pipeline);
        glBindProgramPipeline(m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_vertexShader.GetID()));
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));

        glEnable(GL_DEPTH_TEST);

        // m_albedoTexture.Upload("Assets/Textures/wall.jpg");
        m_albedoTexture.Upload(m_defaultAlbedo, 1, 1, TextureFormat::RGBA);
    }

    BasicLightExample::~BasicLightExample()
    {
        glDeleteProgramPipelines(1, &m_pipeline);
        glDeleteBuffers(1, &m_renderUbo);
        glDeleteBuffers(1, &m_cameraUbo);
        glDeleteVertexArrays(1, &m_planeVAO);
        glDeleteBuffers(1, &m_planeVBO);
        glDeleteBuffers(1, &m_planeEBO);
        glDeleteVertexArrays(1, &m_sphereVAO);
        glDeleteBuffers(1, &m_sphereVBO);
        glDeleteBuffers(1, &m_sphereEBO);
        glDeleteVertexArrays(1, &m_rubberToyVAO);
        glDeleteBuffers(1, &m_rubberToyEBO);
        glDeleteBuffers(1, &m_rubberToyVBO);

        if (m_renderThread)
            m_renderThread->join();
    }

    void BasicLightExample::Render()
    {
        m_camera.Update();

        // 开启面剔除，剔除背面
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // 正面朝向设置为顺时针
        // glFrontFace(GL_CW);

        Color clearColor = DimGray;
        glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_wireframe)
        {
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, CastID<GLuint>(m_wireframeGeometryShader.GetID()));
            glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_wireframeFragmentShader.GetID()));
        }
        else
        {
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, 0);
            glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));
        }

        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUbo);
        CameraData cameraData;
        cameraData.view = m_camera.ViewMatrix();
        cameraData.projection = m_camera.ProjectionMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &cameraData);

        glBindBuffer(GL_UNIFORM_BUFFER, m_renderUbo);
        RenderData renderData;
        renderData.lightDirection = m_lightDirection;
        renderData.lightColor = m_lightColor.ToVector4();
        renderData.viewPosition.Block<3, 1>(0, 0) = m_camera.position;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(RenderData), &renderData);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_albedoTexture.GetID()));

        glBindProgramPipeline(m_pipeline);
        glBindVertexArray(m_sphereVAO);
        // 绘制球
        for (const auto& [position, color, ambientStrength, specularStrength, shininess] : m_sphereProperties)
        {
            Math::Transform<float, 3> modelTransform;
            Math::Vector3f movingPosition = position;
            modelTransform.Translate(movingPosition);
            m_vertexShader.SetMat4("model", modelTransform.ToTransformMatrix());

            m_fragmentShader.SetFloat3("objectColor", color.R(), color.G(), color.B());
            m_fragmentShader.SetFloat("ambientStrength", ambientStrength);
            m_fragmentShader.SetFloat("specularStrength", specularStrength);
            m_fragmentShader.SetFloat("shininess", shininess);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_sphereMesh.indices.size()),
                GL_UNSIGNED_INT, nullptr);
        }
        // 绘制地面
        glBindVertexArray(m_planeVAO);
        Math::Transform<float, 3> planeModelTransform;
        planeModelTransform.Translate(m_planeProperty.position);
        m_vertexShader.SetMat4("model", planeModelTransform.ToTransformMatrix());

        m_fragmentShader.SetFloat3("objectColor",
            m_planeProperty.color.R(), m_planeProperty.color.G(), m_planeProperty.color.B());
        m_fragmentShader.SetFloat("ambientStrength", m_planeProperty.ambientStrength);
        m_fragmentShader.SetFloat("specularStrength", m_planeProperty.specularStrength);
        m_fragmentShader.SetFloat("shininess", m_planeProperty.shininess);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_planeMesh.indices.size()),
            GL_UNSIGNED_INT, nullptr);

        // 绘制橡胶玩具
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_rubberToyAlbedo.GetID()));
        glBindVertexArray(m_rubberToyVAO);
        Math::Transform<float, 3> rubberToyModelTransform;
        rubberToyModelTransform.Translate(m_rubberToyProperty.position);
        rubberToyModelTransform.Scale(2.0f);
        m_vertexShader.SetMat4("model", rubberToyModelTransform.ToTransformMatrix());

        m_fragmentShader.SetFloat3("objectColor",
            m_rubberToyProperty.color.R(), m_rubberToyProperty.color.G(), m_rubberToyProperty.color.B());
        m_fragmentShader.SetFloat("ambientStrength", m_rubberToyProperty.ambientStrength);
        m_fragmentShader.SetFloat("specularStrength", m_rubberToyProperty.specularStrength);
        m_fragmentShader.SetFloat("shininess", m_rubberToyProperty.shininess);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_rubberToyMesh.indices.size()),
            GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        m_viewportGrid.Render(m_camera);
        m_viewportCompass.Render(m_camera);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("General");
        ImGui::Checkbox("Draw wireframe", &m_wireframe);
        ImGui::BeginDisabled(!m_pathTracer.IsReady());
        if (ImGui::Button("Render"))
        {
            if (m_renderThread)
            {
                m_renderThread->join();
                m_renderThread.reset();
            }
            m_renderThread = std::make_unique<std::thread>([this] {
                RayTracing::PathTraceData pathTraceData;
                pathTraceData.imageWidth = 1600;
                pathTraceData.imageHeight = 900;
                pathTraceData.cameraPosition = m_camera.position;
                pathTraceData.front = m_camera.front;
                pathTraceData.up = m_camera.up;
                pathTraceData.right = m_camera.right;
                pathTraceData.aspectRatio = m_camera.AspectRatio();
                pathTraceData.focalDistance = 8.0f;
                pathTraceData.fov = m_camera.fov;
                pathTraceData.defocusAngle = 10.0f;
                pathTraceData.samplesPerPixel = 50;
                pathTraceData.maxDepth = 50;
                m_pathTracer.Run(pathTraceData);
            });
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::ProgressBar(m_pathTracer.Process(), ImVec2(200.0f, 0.0f));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void BasicLightExample::LoadRubberToy()
    {
        // 读取gltf文件，创建Model
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        std::string filename("Assets/Meshes/RubberToy/rubber_toy.gltf");

        bool result = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        if (!warn.empty())
            LogWarn(warn);

        if (!err.empty())
            LogError(err);

        if (!result)
            LogInfo("Failed to load glTF: {}", filename);
        else
            LogInfo("Loaded glTF: {}", filename);

        // 加载Mesh数据
        // 这里先测试最简单的情况
        const tinygltf::Scene scene = model.scenes[model.defaultScene];
        const tinygltf::Node node = model.nodes[0];
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];
        const tinygltf::Primitive& primitive = mesh.primitives[0];
        for (auto attribute : primitive.attributes)
        {
            tinygltf::Accessor accessor = model.accessors[attribute.second];
            const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer buffer = model.buffers[bufferView.buffer];
            std::size_t count = accessor.count;
            auto data = reinterpret_cast<float const*>(buffer.data.data() + bufferView.byteOffset);
            for (int i = 0; i < count; ++i)
            {
                if (attribute.first == "POSITION")
                {
                    Math::Vector3f vertex;
                    vertex.X() = data[3 * i];
                    vertex.Y() = data[3 * i + 1];
                    vertex.Z() = data[3 * i + 2];
                    m_rubberToyMesh.vertices.emplace_back(vertex);
                }
                else if (attribute.first == "NORMAL")
                {
                    Math::Vector3f normal;
                    normal.X() = data[3 * i];
                    normal.Y() = data[3 * i + 1];
                    normal.Z() = data[3 * i + 2];
                    m_rubberToyMesh.normals.emplace_back(normal);
                }
                else if (attribute.first == "TANGENT")
                {
                    Math::Vector3f tangent;
                    tangent.X() = data[4 * i];
                    tangent.Y() = data[4 * i + 1];
                    tangent.Z() = data[4 * i + 2];
                    m_rubberToyMesh.tangents.emplace_back(tangent);
                }
                else if (attribute.first == "TEXCOORD_0")
                {
                    Math::Vector2f uv;
                    uv.X() = data[2 * i];
                    uv.Y() = data[2 * i + 1];
                    m_rubberToyMesh.uvs.emplace_back(uv);
                }
            }
        }
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
        const tinygltf::BufferView indexBufferView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer indexBuffer = model.buffers[indexBufferView.buffer];
        std::size_t indexCount = indexAccessor.count;
        auto data = reinterpret_cast<std::uint32_t const*>(indexBuffer.data.data() + indexBufferView.byteOffset);
        for (int i = 0; i < indexCount; ++i)
            m_rubberToyMesh.indices.emplace_back(data[i]);

        // 创建VAO
        glGenBuffers(1, &m_rubberToyVBO);
        glGenBuffers(1, &m_rubberToyEBO);
        glGenVertexArrays(1, &m_rubberToyVAO);
        glBindVertexArray(m_rubberToyVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_rubberToyVBO);
        auto verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_rubberToyMesh.vertices.size());
        auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_rubberToyMesh.uvs.size());
        auto normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_rubberToyMesh.normals.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_rubberToyMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_rubberToyMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize, m_rubberToyMesh.normals.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rubberToyEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_rubberToyMesh.indices.size()),
            m_rubberToyMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        // 加载Albedo贴图
        tinygltf::Texture texture = model.textures[0];
        tinygltf::Image image = model.images[texture.source];
        auto textureFormat = TextureFormat::RGB;
        if (image.component == 1)
            textureFormat = TextureFormat::R;
        else if (image.component == 2)
            textureFormat = TextureFormat::RG;
        else if (image.component == 3)
            textureFormat = TextureFormat::RGB;
        else if (image.component == 4)
            textureFormat = TextureFormat::RGBA;
        m_rubberToyAlbedo.Upload(image.image.data(), image.width, image.height, textureFormat);
    }
}
