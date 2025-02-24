#include "InstanceShader.h"



InstanceShader::InstanceShader(ID3D11Device* device, HWND hwnd) :BaseShader(device, hwnd)
{
	initShader(L"instance_VS.cso", L"instance_PS.cso");
}


InstanceShader::~InstanceShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (posBuffer)
	{
		posBuffer->Release();
		posBuffer = 0;
	}
	if (timeBuffer)
	{
		timeBuffer->Release();
		timeBuffer = 0;
	}


	BaseShader::~BaseShader();
}

void InstanceShader::initShader(WCHAR* vertex, WCHAR* pixel)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cellPosBufferDesc;
	D3D11_BUFFER_DESC timeBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	load(vertex);
	loadPixelShader(pixel);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	cellPosBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cellPosBufferDesc.ByteWidth = sizeof(cellPosBufferType);
	cellPosBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cellPosBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cellPosBufferDesc.MiscFlags = 0;
	cellPosBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&cellPosBufferDesc, NULL, &posBuffer);

	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(timeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

}

void InstanceShader::setShderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, int indexCount, int instanceCount, ID3D11ShaderResourceView* texture, XMFLOAT2 positions[], float time)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(world);
	tview = XMMatrixTranspose(view);
	tproj = XMMatrixTranspose(projection);

	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//send position data to shader
	cellPosBufferType* posDataPtr;
	deviceContext->Map(posBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	posDataPtr = (cellPosBufferType*)mappedResource.pData;
	for (int i = 0; i < 16; i++)
	{
		posDataPtr->pos[i] = positions[i];
	}
	deviceContext->Unmap(posBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &posBuffer);

	//time no onger used but still gets sent over to buffer
	timeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (timeBufferType*)mappedResource.pData;
	timePtr->time = time;
	timePtr->padding = XMFLOAT3(0, 0, 0);
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &timeBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

}

void InstanceShader::render(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	//render the voxals
	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

//overloaded vertex input buffer - should have made a another base shader class that inhearates from base shader with this in it
bool InstanceShader::load(WCHAR* name)
{
	ID3D10Blob* customeVertexShaderBuffer;
	//set size of input
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	ID3D10Blob* error;
	HRESULT result;

	unsigned int numberElements;

	customeVertexShaderBuffer = 0;

	result = D3DReadFileToBlob(name, &customeVertexShaderBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//create a new vertex shader
	result = renderer->CreateVertexShader(customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);

	if (FAILED(result))
	{

		return false;
	}
	//set up the type of data that we will send to the buffer
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 1;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = 0;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;

	polygonLayout[4].SemanticName = "COLOR";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[4].InputSlot = 1;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[4].InstanceDataStepRate = 1;

	numberElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//create a new input and hand it the new vertex buffer
	result = renderer->CreateInputLayout(polygonLayout, numberElements, customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), &layout);

	if (FAILED(result))
	{
		return false;
	}

	customeVertexShaderBuffer->Release();
	customeVertexShaderBuffer = nullptr;

	return true;
}
