#include "QApplication"
#include "QGraphicsView"
#include "QGraphicsItem"
#include "QMainWindow"
#include "OpenGLWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // QGraphicsScene scene;
    // scene.setSceneRect(0, 0, 400, 300);
    //
    // QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, 100, 50);
    // rect->setPos(50, 50);
    // rect->setBrush(Qt::blue);
    // scene.addItem(rect);
    //
    // QGraphicsView view(&scene);
    // view.setWindowTitle("Qt Graphics View Demo");
    // view.resize(500, 400);
    // view.show();

    OpenGLWidget glWidget;
    glWidget.resize(800, 600);
    glWidget.setWindowTitle("Qt6 OpenGL Demo");
    glWidget.show();

    return QApplication::exec();
}
