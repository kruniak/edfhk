Texture2D<float4> InputTexture : register(t0);
SamplerState LinearSampler : register(s0);

float2 TexelSize; // This should be set to (1.0 / ScreenWidth, 1.0 / ScreenHeight)

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 PSMain(PixelInput input) : SV_TARGET
{
	float4 blurColor = float4(0.0, 0.0, 0.0, 0.0);

	// Apply a box blur by sampling neighboring pixels
	for (int i = -1; i <= 1; i++)
	{
		blurColor += InputTexture.Sample(LinearSampler, input.texCoord + float2(i * TexelSize.x, 0.0));
	}

	blurColor /= 3.0; // Adjust to control the blur intensity

	return blurColor;
}
