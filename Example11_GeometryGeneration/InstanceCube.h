#pragma once
#include "BaseMesh.h"
#include "D3D.h"
class InstanceCube : public BaseMesh
{
public:
	InstanceCube(ID3D11Device* device);
	~InstanceCube();
	void sendData(ID3D11DeviceContext* deviceContext);
	int getInstanceCount() { return instanceCount; }
	int getVertexCount() { return vertexCount; }
protected:

	void initBuffers(ID3D11Device* device);
	int instanceCount;

};

