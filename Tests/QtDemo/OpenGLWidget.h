#pragma once

#include "QOpenGLWidget"
#include "QOpenGLFunctions_4_5_Core"
#include "QOpenGLShaderProgram"
#include <memory>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    GLuint m_VAO{ 0 }, m_VBO{ 0 };
};
