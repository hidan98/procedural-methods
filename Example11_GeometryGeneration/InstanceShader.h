#pragma once
#include"DXF.h"

using namespace std;
using namespace DirectX;

class InstanceShader : public BaseShader
{
public:
	InstanceShader(ID3D11Device* device, HWND hwnd);
	~InstanceShader();

	void setShderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, int indexCount, int instanceCount, ID3D11ShaderResourceView* texture);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount, int instanceCount);
private:

	void initShader(WCHAR*, WCHAR*);

	bool load(WCHAR* name);

};

