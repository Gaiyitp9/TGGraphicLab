#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    setFormat(format);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    m_program = std::make_unique<QOpenGLShaderProgram>(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "triangle.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "triangle.frag");
    if (!m_program->link())
        qWarning() << "Shader link error: " << m_program->log();

    constexpr float vertices[] = {
        // 位置             // 颜色
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // 右下
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // 顶部
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性（layout = 0）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // 颜色属性（layout = 1）
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    m_program->release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
