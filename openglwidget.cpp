#include "openglwidget.h"
#include <QOpenGLExtraFunctions>
#include <qmath.h>

#include <QtMath>
#include <QVector>
OpenglWidget::OpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),index(QOpenGLBuffer::IndexBuffer),plotIndex(QOpenGLBuffer::IndexBuffer)
{

    xAxis = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    yAxis = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    lastX = size().width() / 2.0f;
    lastY = size().height() / 2.0f;
    color = QVector3D(1.0f,0.2f,0.5f);

}

void OpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    initShaders();
    initAxis();
    updatePlotBuffer();
}

void OpenglWidget::resizeGL(int w, int h)
{
    projectionMatrix.setToIdentity();
}

void OpenglWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();

    shader.bind();

    shader.setUniformValue("viewMatrix", viewMatrix);
    shader.setUniformValue("modelMatrix", modelMatrix);
    shader.setUniformValue("projectionMatrix", projectionMatrix);
    shader.setUniformValue("color",color);

    plot.bind();
    int vertLoc = shader.attributeLocation("a_Vertex");
    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, 0, 2, sizeof(QVector2D));

    glDrawArrays(GL_LINE_STRIP,0,plot.size()/8);

    xAxis->bind();

    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, 0, 2, sizeof(QVector2D));

    index.bind();
    glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,0);
    index.release();

    yAxis->bind();
    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, 0, 2, sizeof(QVector2D));
    index.bind();
    glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,0);
    index.release();
    yAxis->release();

}

void OpenglWidget::initShaders()
{
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,"C:\\Users\\ea56\\Documents\\Plot\\vert.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,"C:\\Users\\ea56\\Documents\\Plot\\frag.frag");
    shader.link();
}



void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (firstMouse)
    {
        lastX = event->localPos().x();
        lastY = event->localPos().y();
        firstMouse = false;
    }

    float xoffset = event->localPos().x() - lastX;
    float yoffset = lastY - event->localPos().y();

    lastX = event->localPos().x();
    lastY = event->localPos().y();

    if (event->buttons() == Qt::LeftButton)
    {
        this->update();
    }
}

void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress)
     {
         int ikey = event->key();
         if(ikey > 1024)
             return;
         keys[ikey] = true;
     }
     this->update();
}

void OpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    int ikey = event->key();
    if(ikey > 1024)
        return;
    keys[ikey] = false;
    this->update();
}

void OpenglWidget::updatePlotBuffer()
{
    int maxLen =  qFabs(xMax) > qFabs(xMin) ? qFabs(xMax) : qFabs(xMin);
    maxLen/=10;
    float fixedStep = 1.0f;

    QVector<QVector2D> data;


    int count = 0;
    for(float i = xMin ; i < xMax ; i += fixedStep){    
        float temp;
        if(!my)
           temp = baseFunc(i);
        else
           temp = myFunc(i);

        qDebug() << i*10.0f << " " <<temp;
        data.push_back(QVector2D(x/100, temp/100));

        ++count;
    }
    plot.create();
    plot.bind();
    plot.allocate(data.data(),data.size() * sizeof(QVector2D));
    plot.release();

    update();
}

void OpenglWidget::initAxis()
{
    QVector<QVector2D> data;

    data.append(QVector2D(-this->size().width()  ,0));
    data.append(QVector2D(this->size().width(),0));

    xAxis->create();
    xAxis->bind();
    xAxis->allocate(data.constData(),data.size() * sizeof(QVector2D));
    xAxis->release();
    data.clear();

    data.append(QVector2D(0,-this->size().height()));
    data.append(QVector2D(0,this->size().height()));

    yAxis->create();
    yAxis->bind();
    yAxis->allocate(data.constData(),data.size() * sizeof(QVector2D));
    yAxis->release();
    QVector<int> indices;
    indices.append(0);
    indices.append(1);
    index.create();
    index.bind();
    index.allocate(indices.constData(),indices.size() * sizeof(int));
    index.release();
}

float OpenglWidget::baseFunc(float x)
{
    return  powf(x,2) - 10;
}

float OpenglWidget::myFunc(float x)
{
    return   qFabs(x - 10) - 10;
}

void OpenglWidget::setYMin(float _yMin)
{
    yMin = _yMin;
    updatePlotBuffer();
}

void OpenglWidget::setYMax(float _yMax)
{
    yMax = _yMax;
    updatePlotBuffer();
}

void OpenglWidget::setXMin(float _xMin)
{
    xMin = _xMin;
    updatePlotBuffer();
}

void OpenglWidget::setXMax(float _xMax)
{
    xMax = _xMax;
    updatePlotBuffer();
}

void OpenglWidget::myStateChagne(int state)
{
    my = state;
    updatePlotBuffer();

}

void OpenglWidget::setColor(const QColor &_color)
{
    color = QVector3D(_color.redF(),_color.greenF(),_color.blueF());
}

void OpenglWidget::getPoint(QVector2D point){
    int maxLen =  qFabs(xMax) > qFabs(xMin) ? qFabs(xMax) : qFabs(xMin);
    maxLen/=10;
    float fixedStep = 1.0f;


    int count = 0;
    for(float i = xMin ; i < xMax ; i += fixedStep){
        float temp;
        if(!my)
           temp = baseFunc(i);
        else
           temp = myFunc(i);

        data.push_back(QVector2D(point.x()/50, point.y()/10 ));

        ++count;
    }
    qDebug()<<point.y();
    plot.create();
    plot.bind();
    plot.allocate(data.data(),data.size() * sizeof(QVector2D));
    plot.release();

    update();

}




