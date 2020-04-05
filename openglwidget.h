#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QOpenGLVertexArrayObject>



class OpenglWidget : public QOpenGLWidget , public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenglWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();


    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void updatePlotBuffer();
    void initAxis();
    float baseFunc(float x);
    float myFunc(float x);


public:
    void setYMin(float _yMin);
    void setYMax(float _yMax);
    void setXMin(float _xMin);
    void setXMax(float _xMax);
    void myStateChagne(int state);
public slots:
    void setColor(const QColor &_color);

private:

    QMatrix4x4 projectionMatrix;
    QQuaternion q;
    float x{0},y{0},z{0};

    QOpenGLShaderProgram shader;
    bool my = false;
    int lastX;
    int lastY;
    bool firstMouse{true};
    float yMin{-80},yMax{80};
    float xMin{-15},xMax{15};
    QOpenGLBuffer *yAxis;
    QOpenGLBuffer *xAxis;
    QOpenGLBuffer index;

    QOpenGLBuffer plot;
    QOpenGLBuffer plotIndex;
    QOpenGLVertexArrayObject vao;
    bool keys[4024] = { false };
    float deltaTime = 1 / 60.0f;

    QVector3D color;



};


#endif // OPENGLWIDGET_H
