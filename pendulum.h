#ifndef PENDULUM_H
#define PENDULUM_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QOpenGLVertexArrayObject>
#include <camera.h>
#include <QtMath>

void initSphere(QOpenGLBuffer *buff,QOpenGLBuffer *ibo, const float& radius, const int& sectorCount, const int &stackCount);
void initCylinder(QOpenGLBuffer *buff,float radius, float height, float halfLength,int slices);

struct PointData
{
    QVector3D vertex;
    QVector3D normal;
    PointData(QVector3D &&vert,QVector3D &&norm)
    {
        vertex = std::move(vert);
        normal = std::move(norm);
    }
    PointData(QVector3D &&vert)
    {
        vertex = std::move(vert);
    }
};


class Pendulum : public QOpenGLWidget , public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Pendulum(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();

    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void updatePendulum();
    void pollEvents();

signals:
    void sendPoint(QVector2D point);

private:

    QMatrix4x4 projectionMatrix;
    QQuaternion q;
    QOpenGLShaderProgram shader;

    int lastX;
    int lastY;
    bool firstMouse{true};

    QOpenGLBuffer vboCylinder;
    QOpenGLBuffer vboSphere;
    QOpenGLBuffer index;

    float angle0 = {-60.0f};
    float angle = angle0;
    float length= {4.0f};
    float mass = 4.0f;
    QVector3D pos{QVector3D(qSin(qDegreesToRadians(angle0)) * length,length + qCos(qDegreesToRadians(angle0)) * length,-10.0f)};
    float g = {9.8f};
    float t = 0.001f;

    float alpha = 0.1f;
    bool keys[4024] = { false };
    float deltaTime = 1 / 60.0f;

    QVector3D color{};
    Camera *camera;
};

#endif // PENDULUM_H
