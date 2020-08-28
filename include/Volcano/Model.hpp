//
//
#ifndef VOLCANO_MODEL_HPP
#define VOLCANO_MODEL_HPP

#include <QVector>

#include <Volcano/Common.hpp>
#include <Volcano/Resource.hpp>
#include <Volcano/OpenGL/Buffer.hpp>

VOLCANO_BEGIN

class VOLCANO_API Model: public Resource
{
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model(void) override;

protected:
    void onSourceChanged(const QUrl &url) override;

private:
    void release(void);

private:
    struct Mesh
    {
        OpenGL::Buffer *vertexBuffer;
        int vertexCount;
        OpenGL::Buffer *vertexIndexBuffer;
        int vertexIndexCount;
    };

    typedef QVector<Mesh> MeshArray;

private:
    MeshArray m_meshArray;
};

VOLCANO_END

#endif // VOLCANO_MODEL_HPP
