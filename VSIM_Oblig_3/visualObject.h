#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "vector3d.h"

class Vertex;
class Texture;
class Shader;


class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
	VisualObject();
	VisualObject(Shader* shader);
	VisualObject(Shader* shader, Texture* texture);
	~VisualObject();
	virtual void init();
	virtual void draw();

	std::pair<float, float> getPosition2D();

	void UpdateTransform();

	QMatrix4x4 GetPosition() const;
	QVector3D GetPosition();
	void SetPosition(const QVector3D& position);
	void SetPosition(const QMatrix4x4& position);
	virtual void move(float dx, float dy, float dz);

	QVector3D GetScale() const;
	void SetScale(const QVector3D& scale);
	void SetScale(const QMatrix4x4& scale);

	QMatrix4x4 GetRotation() const;
	void SetRotation(const QVector3D& rotation);
	void rotate(float dx, float dy, float dz);

protected:
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	GLuint mEAB{ 0 };
	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
	GLint mTextureUniform{ 0 };
	GLint mMatrixUniform{ 0 };
	QMatrix4x4 mMatrix;

	QMatrix4x4 mPosition;
	QMatrix4x4 mRotation;
	QMatrix4x4 mScale;

	QVector3D mVelocity;

public:
	Shader* mShader;
	Texture* mTexture;
};

#endif // VISUALOBJECT_H