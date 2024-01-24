#pragma once

namespace dx11::shaders
{
	// compile custom shaders at runtime
	ID3D11PixelShader* CompileShader(ID3D11Device* pDevice, const char* shaderSource)
	{
		ID3D11PixelShader* customShader = nullptr;
		ID3DBlob* customShaderBytecode = nullptr;
		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompile(shaderSource, strlen(shaderSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &customShaderBytecode, &errorBlob);

		if (SUCCEEDED(hr))
		{
			hr = pDevice->CreatePixelShader(customShaderBytecode->GetBufferPointer(), customShaderBytecode->GetBufferSize(), nullptr, &customShader);
		}
		else
		{
			if (errorBlob)
			{
				OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
				errorBlob->Release();
			}
		}

		return customShader;
	}
}

