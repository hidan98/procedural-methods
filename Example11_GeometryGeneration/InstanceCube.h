#pragma once
#include "BaseMesh.h"
#include "D3D.h"

struct cells;

class InstanceCube : public BaseMesh
{
public:
	InstanceCube(ID3D11Device* device);
	~InstanceCube();
	void sendData(ID3D11DeviceContext* deviceContext);
	void init(ID3D11Device* device, cells* cellMap, int count);
	void Init2D(ID3D11Device* device, cells* cellMap, int count, int width, int depth);
	int getInstanceCount() { return instanceCount; }
	int getVertexCount() { return vertexCount; }
protected:

	void initBuffers(ID3D11Device* device);
	int instanceCount;

	int it;

};

