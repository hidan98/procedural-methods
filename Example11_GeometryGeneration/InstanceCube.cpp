#include "InstanceCube.h"
#include "Cave.h"



InstanceCube::InstanceCube(ID3D11Device* device)
{
	initBuffers(device);
}


InstanceCube::~InstanceCube()
{
	BaseMesh::~BaseMesh();
}
void InstanceCube::init(ID3D11Device* device, cells* cellMap, int count, int width, int depth, int height)
{
	//initBuffers(device);
	D3D11_SUBRESOURCE_DATA instanceData;

	instanceCount = count;
	
	InstanceType* instances = new InstanceType[instanceCount];

	int index = 0;
	int pos = 0;
	it = 0;

	for (int z = 0; z < depth; z++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				index = (z * depth + x);
				index = (z * height * width) + (y*width) + x;
				it++;
				if (cellMap[index].active)
				{
					instances[pos].position = cellMap[index].position;
					instances[pos].colour = cellMap[index].colour;
					pos++;
				}
			}

		}
	}

	D3D11_BUFFER_DESC instanceBufferDesc = { sizeof(InstanceType)* instanceCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	instanceData = { instances ,0,0 };
	device->CreateBuffer(&instanceBufferDesc, &instanceData, &instanceBuffer);

	delete[] instances;
	instances = 0;
}
void InstanceCube::initBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	vertexCount = 36;
	indexCount = 36;

	VertexType* vertices = new VertexType[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	//front face
	vertices[0].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[0].texture = XMFLOAT2(0.f, 0.0f);
	indices[0] = 0;

	vertices[1].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[1].texture = XMFLOAT2(0.0f, 1.0f);
	indices[1] = 1;

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	indices[2] = 2;

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[3].texture = XMFLOAT2(1.f, 0.0f);
	indices[3] = 3;

	vertices[4].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[4].texture = XMFLOAT2(0.0f, 1.0f);
	indices[4] = 4;

	vertices[5].position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	indices[5] = 5;



	//top face
	vertices[6].position = XMFLOAT3(0.0f, 1.0f, 1.0f);
	vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[6].texture = XMFLOAT2(0.0f, 0.0f);
	indices[6] = 6;

	vertices[7].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[7].texture = XMFLOAT2(0.0f, 1.0f);
	indices[7] = 7;

	vertices[8].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[8].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[8].texture = XMFLOAT2(1.0f, 0.0f);
	indices[8] = 8;

	vertices[9].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[9].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[9].texture = XMFLOAT2(1.0f, 0.0f);
	indices[9] = 9;

	vertices[10].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[10].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[10].texture = XMFLOAT2(0.0f, 1.0f);
	indices[10] = 10;

	vertices[11].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[11].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[11].texture = XMFLOAT2(1.0f, 1.0f);
	indices[11] = 11;



	//back face

	vertices[12].position = XMFLOAT3(0.0f, 1.0f, 1.0f);
	vertices[12].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[12].texture = XMFLOAT2(0.0f, 0.0f);
	indices[12] = 17;

	vertices[13].position = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[13].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[13].texture = XMFLOAT2(0.0f, 1.0f);
	indices[13] = 16;

	vertices[14].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[14].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[14].texture = XMFLOAT2(1.0f, 0.0f);
	indices[14] = 15;

	vertices[15].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[15].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[15].texture = XMFLOAT2(1.f, 0.0f);
	indices[15] = 14;

	vertices[16].position = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[16].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[16].texture = XMFLOAT2(0.0f, 1.0f);
	indices[16] = 13;

	vertices[17].position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	vertices[17].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[17].texture = XMFLOAT2(1.0f, 1.0f);
	indices[17] = 12;

	//bottom face

	vertices[18].position = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[18].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[18].texture = XMFLOAT2(0.0f, 0.0f);
	indices[18] = 23;

	vertices[19].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[19].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[19].texture = XMFLOAT2(0.0f, 1.0f);
	indices[19] = 22;

	vertices[20].position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	vertices[20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[20].texture = XMFLOAT2(1.0f, 0.0f);
	indices[20] = 21;

	vertices[21].position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	vertices[21].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[21].texture = XMFLOAT2(1.0f, 0.0f);
	indices[21] = 20;

	vertices[22].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[22].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[22].texture = XMFLOAT2(0.0f, 1.0f);
	indices[22] = 19;

	vertices[23].position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[23].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[23].texture = XMFLOAT2(1.0f, 1.0f);
	indices[23] = 18;

	//left face

	vertices[24].position = XMFLOAT3(0.0f, 1.0f, 1.0f);
	vertices[24].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[24].texture = XMFLOAT2(0.0f, 0.0f);
	indices[24] = 24;

	vertices[25].position = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[25].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[25].texture = XMFLOAT2(0.0f, 1.0f);
	indices[25] = 25;

	vertices[26].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[26].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[26].texture = XMFLOAT2(1.0f, 0.0f);
	indices[26] = 26;

	vertices[27].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[27].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[27].texture = XMFLOAT2(1.0f, 0.0f);
	indices[27] = 27;

	vertices[28].position = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[28].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[28].texture = XMFLOAT2(0.0f, 1.0f);
	indices[28] = 28;

	vertices[29].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[29].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[29].texture = XMFLOAT2(1.0f, 1.0f);
	indices[29] = 29;


	//right face
	vertices[30].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[30].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[30].texture = XMFLOAT2(0.0f, 0.0f);
	indices[30] = 35;

	vertices[31].position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	vertices[31].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[31].texture = XMFLOAT2(0.0f, 1.0f);
	indices[31] = 34;

	vertices[32].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[32].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[32].texture = XMFLOAT2(1.0f, 0.0f);
	indices[32] = 33;

	vertices[33].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[33].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[33].texture = XMFLOAT2(1.0f, 0.0f);
	indices[33] = 32;

	vertices[34].position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	vertices[34].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[34].texture = XMFLOAT2(0.0f, 1.0f);
	indices[34] = 31;

	vertices[35].position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[35].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[35].texture = XMFLOAT2(1.0f, 1.0f);
	indices[35] = 30;

	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

void InstanceCube::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride[2];
	unsigned int offset[2];
	ID3D11Buffer* bufferPointers[2];

	// Set vertex buffer stride and offset.
	stride[0] = sizeof(VertexType);
	stride[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offset[0] = 0;
	offset[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}