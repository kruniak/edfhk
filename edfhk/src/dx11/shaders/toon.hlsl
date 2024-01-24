// Basic toon shader that applies a simple outline effect and flat shading.
// Replace with your custom shader logic.

//Texture2D<float4> MainTexture : register(t0);
//SamplerState MainSampler : register(s0);
//
//struct PixelInput
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR0;
//};
//
//float4 main(PixelInput input) : SV_TARGET
//{
//    // Compute flat shading using the dot product of the normal and light direction.
//    float3 lightDir = normalize(float3(1.0, 1.0, 1.0)); // Example light direction.
//    float3 normal = normalize(cross(ddy(input.position.xyz), ddx(input.position.xyz)));
//    float shading = saturate(dot(normal, lightDir));
//
//    // Apply a simple outline effect based on shading.
//    float outline = 1.0 - smoothstep(0.4, 0.5, shading);
//
//    // Apply the shading to the pixel color.
//    float4 color = input.color * shading;
//
//    // Combine shading and outline.
//    color.rgb *= outline;
//
//    return color;
//}
//
//Texture2D<float4> MainTexture : register(t0);
//SamplerState MainSampler : register(s0);
//
//struct PixelInput
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR0;
//};
//
//float4 main(PixelInput input) : SV_Target
//{
//    // Compute flat shading using the dot product of the normal and light direction.
//    float3 lightDir = normalize(float3(1.0, 1.0, 1.0)); // Example light direction.
//    float3 normal = normalize(cross(ddy(input.position.xyz), ddx(input.position.xyz))).xyz;
//    float shading = saturate(dot(normal, lightDir));
//
//    // Apply a simple outline effect based on shading.
//    float outline = 1.0 - smoothstep(0.4, 0.5, shading);
//
//    // Apply the shading to the pixel color.
//    float3 shadedColor = input.color.rgb * shading;
//
//    // Combine shading and outline.
//    float3 finalColor = shadedColor * outline;
//
//    return float4(finalColor, input.color.a);
//}

Texture2D<float4> MainTexture : register(t0);
SamplerState MainSampler : register(s0);

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 main(PixelInput input) : SV_Target
{
    // Compute flat shading using the dot product of the normal and light direction.
    float3 lightDir = normalize(float3(1.0, 1.0, 1.0)); // Example light direction.
    float3 normal = normalize(cross(ddy(input.position.xyz), ddx(input.position.xyz))).xyz;
    float shading = saturate(dot(normal, lightDir));

    // Apply a simple outline effect based on shading.
    float outline = 1.0 - smoothstep(0.4, 0.5, shading);

    // Apply the shading to the pixel color.
    float3 shadedColor = input.color.rgb * shading;

    // Combine shading and outline.
    float3 finalColor = shadedColor * outline;

    return float4(finalColor, input.color.a);
}

