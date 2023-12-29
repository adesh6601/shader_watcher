#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "Line.h"
#include <vector>
#include "Point3D.h"

class QOpenGLTexture;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLPaintDevice;

class OpenGLWindow :public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
signals:
    void shapeUpdate();
public:
    OpenGLWindow(const QColor& background, QMainWindow* parent);
    ~OpenGLWindow();
    void mouseMoveEvent(QMouseEvent* event);

public:
    int gridSize;
    void addLine(Point3D p1, Point3D p2);
    void addGrid(int size);
    void simpleDDA(Point3D inP1, Point3D inP2);
    void symmetricDDA(Point3D inP1, Point3D inP2);
    void bazierCurve(std::vector<Point3D> v);
    void hermiteCurve(std::vector<Point3D> v);

protected:
    void paintGL() override;
    void initializeGL() override;

private:
    void reset();
    void fillSquare(const QVector<QVector2D>& squareVertices, const QVector3D& fillColor);
    void colorSqr(QVector<QVector2D>& pixels);
    QString readShader(QString filePath);

private slots:
    void shaderWatcher();

private:
    QOpenGLShader* mVshader = nullptr;
    QOpenGLShader* mFshader = nullptr;
    QOpenGLShaderProgram* mProgram = nullptr;
    QOpenGLBuffer mVbo;
    int mVertexAttr;
    int mNormalAttr;
    int mMatrixUniform;
    QColor mBackground;
    QMetaObject::Connection mContextWatchConnection;
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    QVector<QVector2D> mPixelVertices;
    QVector<GLfloat> curveVertices;
    QVector<GLfloat> curveColors;
    QQuaternion rotationAngle;
    QPoint lastPos;
    QVector<GLfloat> mVertices;
    QVector<GLfloat> mColors;
    QFileSystemWatcher *mShaderWatcher;
};