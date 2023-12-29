#include "stdafx.h"
#include "OpenGLWindow.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include "Grid.h"
#include "SimpleDDA.h"
#include "Line.h"
#include "SymmetricDDA.h"
#include "Bezier.h"
#include "Hermite.h"
#include "qfile.h"

OpenGLWindow::OpenGLWindow(const QColor& background, QMainWindow* parent) :
    mBackground(background)
{
    setParent(parent);
    setMinimumSize(720, 550);

    QStringList filePathList = { "D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\vertexShaderSource.glsl",
                              "D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\fragmentShaderSource.glsl"};
    mShaderWatcher = new QFileSystemWatcher(filePathList, this);

    connect(mShaderWatcher, &QFileSystemWatcher::fileChanged, this, &OpenGLWindow::shaderWatcher);
}

OpenGLWindow::~OpenGLWindow()
{
    reset();
}

void OpenGLWindow::reset()
{
    // And now release all OpenGL resources.
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();

    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    mProgram->bind();

    QMatrix4x4 matrix;
    matrix.ortho(-3.0f - (gridSize / 2), 3.0f + (gridSize / 2), -3.0f - (gridSize / 2), 3.0f + (gridSize / 2), 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(rotationAngle);
    mProgram->setUniformValue(m_matrixUniform, matrix);
    std::vector<double> vert;
    std::vector<double> col;

    Grid grid;
    grid.drawGrid(gridSize, vert, col);

    for (int i = 0; i < vert.size(); i++) {
        mVertices.append(vert[i]);
    }
    for (int i = 0; i < col.size(); i++) {
        mColors.append(col[i]);
    }

    colorSqr(mPixelVertices);

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, mVertices.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());


    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_LINES, 0, mVertices.size() / 2);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

}

void OpenGLWindow::colorSqr(QVector<QVector2D>& pixels)
{
    int i = 0;
    while (i < pixels.size()) {
        QVector<QVector2D> qv;
        qv.append(pixels[i]);
        i++;
        qv.append(pixels[i]);
        i++;
        qv.append(pixels[i]);
        i++;
        qv.append(pixels[i]);
        i++;
        QVector3D fillColor(1.0f, 1.0f, 0.0f);
        fillSquare(qv, fillColor);
    }
}

void OpenGLWindow::fillSquare(const QVector<QVector2D>& squareVertices, const QVector3D& fillColor)
{

    QVector<GLfloat> mVertices;
    QVector<GLfloat> mColors;

    // Convert QVector<QVector2D> to QVector<GLfloat>
    for (const auto& vertex : squareVertices)
    {
        mVertices << vertex.x();
        mVertices << vertex.y();
        mColors << fillColor.x() << fillColor.y() << fillColor.z();
    }

    // Prepare the data for rendering
    GLfloat* mVerticesData = mVertices.data();
    GLfloat* mColorsData = mColors.data();

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, mVerticesData);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColorsData);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size() / 2);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

}

void OpenGLWindow::initializeGL()
{
    QString vertexShaderSource = readShader("D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\vertexShaderSource.glsl");
    QString fragmentShaderSource = readShader("D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\fragmentShaderSource.glsl");

    //static const char* vertexShaderSource =
    //  "attribute highp vec4 posAttr;\n"
    //  "attribute lowp vec4 colAttr;\n"
    //  "varying lowp vec4 col;\n"
    //  "uniform highp mat4 matrix;\n"
    //  "void main() {\n"
    //  "   col = colAttr;\n"
    //  "   gl_Position = matrix * posAttr;\n"
    //  "}\n";

    //static const char* fragmentShaderSource =
    //  "varying lowp vec4 col;\n"
    //  "void main() {\n"
    //  "   gl_FragColor = col;\n"
    //  "}\n";

    initializeOpenGLFunctions();

    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();
    m_posAttr = mProgram->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = mProgram->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = mProgram->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);

        rotationAngle = rotX * rotY * rotationAngle;
        update();
    }
    lastPos = event->pos();
}

void OpenGLWindow::addLine(Point3D p1, Point3D p2)
{
    mVertices.push_back(p1.x());
    mVertices.push_back(p1.y());
    mVertices.push_back(p2.x());
    mVertices.push_back(p2.y());

    mColors << 1.0f << 0.0f << 0.0f;
    mColors << 1.0f << 0.0f << 0.0f;

    emit shapeUpdate();
}

void OpenGLWindow::addGrid(int size)
{
    gridSize = size;
    emit shapeUpdate();
}

void OpenGLWindow::simpleDDA(Point3D inP1, Point3D inP2)
{
    mPixelVertices.clear();
    std::vector<Point3D> allPoints;

    SimpleDDA line;
    line.plotLine(inP1, inP2, allPoints);

    for (Point3D point: allPoints)
    {
        QVector2D pixelPoint;
        pixelPoint.setX(point.x());
        pixelPoint.setY(point.y());
        mPixelVertices.append(pixelPoint);
    }
    colorSqr(mPixelVertices);
    emit shapeUpdate();
    addLine(inP1, inP2);
}

void OpenGLWindow::symmetricDDA(Point3D inP1, Point3D inP2)
{
    mPixelVertices.clear();
    std::vector<Point3D> allPoints;

    SymmetricDDA line;
    line.plotLine(inP1, inP2, allPoints);

    for (Point3D point : allPoints)
    {
        QVector2D pixelPoint;
        pixelPoint.setX(point.x());
        pixelPoint.setY(point.y());
        mPixelVertices.append(pixelPoint);
    }
    colorSqr(mPixelVertices);
    emit shapeUpdate();
    addLine(inP1, inP2);
}

void OpenGLWindow::bazierCurve(std::vector<Point3D> allPoints)
{
    std::vector<float> mVertices;
    std::vector<float> mColors;

    Bezier curve;
    curve.drawCurve(allPoints[0], allPoints[1], allPoints[2], allPoints[3], mVertices, mColors);
    
    curveVertices.clear();
    curveColors.clear();

    curveVertices = QVector<GLfloat>(mVertices.begin(), mVertices.end());
    curveColors = QVector<GLfloat>(mColors.begin(), mColors.end());

    emit shapeUpdate();
}

void OpenGLWindow::hermiteCurve(std::vector<Point3D> allPoints)
{
    std::vector<float> mVertices;
    std::vector<float> mColors;

    Hermite curve;
    curve.drawCurve(allPoints[0], allPoints[1], allPoints[2], allPoints[3], mVertices, mColors);

    curveVertices.clear();
    curveColors.clear();

    curveVertices = QVector<GLfloat>(mVertices.begin(), mVertices.end());
    curveColors = QVector<GLfloat>(mColors.begin(), mColors.end());

    emit shapeUpdate();
}

QString OpenGLWindow::readShader(QString filePath)
{
    QString shader = "INVALID SHADER";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        shader.clear();
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            shader.append(stream.readLine());
        }
    }
    return shader;
}

void OpenGLWindow::shaderWatcher()
{
    QString vertexShaderSource = readShader("D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\vertexShaderSource.glsl");
    QString fragmentShaderSource = readShader("D:\\adesh_workspace\\Visualiser\\visualizer\\resource\\fragmentShaderSource.glsl");

    mProgram->release();
    mProgram->removeAllShaders();
    mProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();
}
