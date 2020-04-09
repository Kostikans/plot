#include "pendulum.h"
#include <QVector>

Pendulum::Pendulum(QWidget *parent)
    : QOpenGLWidget(parent),index(QOpenGLBuffer::IndexBuffer)
{

    lastX = size().width() / 2.0f;
    lastY = size().height() / 2.0f;
    color = QVector3D(1.0f,0.2f,0.5f);
    camera = new Camera(QVector3D(0.0f,0.0f,1.0f));

    this->setMouseTracking(true);
    this->grabKeyboard();

}

void Pendulum::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    initSphere(&vboSphere, &index,1.0f,100,100);
    initCylinder(&vboCylinder,0.2f,2.0f,0.2f,50);
    initShaders();

}

void Pendulum::resizeGL(int w, int h)
{
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void Pendulum::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(pos);

    shader.bind();
    pollEvents();
    shader.setUniformValue("viewMatrix", camera->GetViewMatrix());
    shader.setUniformValue("modelMatrix", modelMatrix);
    shader.setUniformValue("projectionMatrix", projectionMatrix);

    vboSphere.bind();
    int offset = 0;
    int vertLoc = shader.attributeLocation("a_Vertex");
    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));


    vboSphere.bind();
    index.bind();
    glDrawElements(GL_TRIANGLES,index.size(),GL_UNSIGNED_INT,0);
    index.release();
    vboSphere.release();

    modelMatrix.setToIdentity();
     modelMatrix.translate(QVector3D(0.0f,0.0f,-5.0f));

    q = QQuaternion();
     q *= QQuaternion::fromAxisAndAngle(QVector3D(1.0f,0.0f,0.0f),-90);
     q *= QQuaternion::fromAxisAndAngle(QVector3D(0.0f,-1.0f,0.0f),angle);

      QVector3D rotated_point = QVector3D(0.0f,0.0f,-5.0f) + (q * (QVector3D(0.0f,-4.0f,-5.0f)-QVector3D(0.0f,0.0f,-5.0f)));
    modelMatrix.rotate(q);
     modelMatrix.translate(rotated_point);

    shader.setUniformValue("modelMatrix", modelMatrix);

    vboCylinder.bind();
    vertLoc = shader.attributeLocation("a_Vertex");
    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

    vboCylinder.bind();
    glDrawArrays(GL_TRIANGLE_FAN,0,vboCylinder.size());
    vboCylinder.release();
    updatePendulum();
    emit sendPoint(QVector2D(t,pos.x()));
}


void Pendulum::initShaders()
{
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,"C:\\Users\\ea56\\Documents\\Plot\\sphereVert.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,"C:\\Users\\ea56\\Documents\\Plot\\sphereFrag.frag");
    shader.link();
}

void initCylinder(QOpenGLBuffer *buff,float radius, float height, float halfLength,int slices)
{
    QVector<PointData> pData;
    for(int i=0; i<slices; i++)
    {
        float seta = i*360.0/slices;
        float vx = sin(qDegreesToRadians(seta))*radius;
        float vy = cos(qDegreesToRadians(seta))*radius;

        pData.push_back(QVector3D(vx, vy,  -height));
        pData.push_back(QVector3D(vx, vy,   height));

    }
    for(int i=0; i<slices; i++)
    {
        float seta = i*360.0/slices;
        float seta1 = (i +1)*360.0/slices;
        float vx = sin(qDegreesToRadians(seta))*radius;
        float vy = cos(qDegreesToRadians(seta))*radius;

        float vx1 = sin(qDegreesToRadians(seta1))*radius;
        float vy1 = cos(qDegreesToRadians(seta1))*radius;

        pData.push_back(QVector3D(vx, vy,  height));
        pData.push_back(QVector3D(vx1, vy1,   height));

        pData.push_back(QVector3D(vx, vy,  -height));
        pData.push_back(QVector3D(vx1, vy1,   -height));

    }
    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();
}

void initSphere(QOpenGLBuffer *buff,QOpenGLBuffer *ibo, const float& radius, const int& sectorCount, const int &stackCount)
{

    QVector<PointData> pData;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            pData.push_back(PointData(QVector3D(x,y,z),QVector3D(nx,ny,nz)));

        }

    }

    QVector<int> indices;
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }


    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();

    ibo->create();
    ibo->bind();
    ibo->allocate(indices.constData(),indices.size() * sizeof(int));
    ibo->release();
}

void Pendulum::mouseMoveEvent(QMouseEvent *event)
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
         camera->ProcessMouseMovement(xoffset, yoffset);
        this->update();
    }
}

void Pendulum::keyPressEvent(QKeyEvent *event)
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

void Pendulum::keyReleaseEvent(QKeyEvent *event)
{
    int ikey = event->key();
    if(ikey > 1024)
        return;
    keys[ikey] = false;
    this->update();
}

void Pendulum::updatePendulum()
{
    angle = angle0 * qExp(-alpha*t) * qCos(sqrtf(g/length)*t + angle0);
    t+=0.01f;


    pos = QVector3D(qSin(qDegreesToRadians(angle)) * length,-qCos(qDegreesToRadians(angle)) * length,-5.0f);

}

void Pendulum::pollEvents()
{
    if (keys[Qt::Key_W] == true)
    {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[Qt::Key_S] == true)
    {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[Qt::Key_A] == true)
    {
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[Qt::Key_D] == true)
    {
        camera->ProcessKeyboard(RIGHT, deltaTime);

    }
    this->update();
}

