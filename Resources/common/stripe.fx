
// Voodoo Shader
// Demo Effect: NLPP
// This effect demonstrates the use of parameter-created textures for deferred rendering.

// Link to the last pass:
Texture2D mainTexture < string vs_texture = ":pass0"; >;
sampler mainSampler = sampler_state { Texture = <mainTexture>; };

// Create a new texture:
Texture2D defTexture
<
	string vs_texture = "demo_defTexture";
	bool vs_texcreate = true; // Texture creation flag, must be true for a new texture.
	int2 vs_texdim = {1024, 768}; // Texture size, required.
	string vs_texfmt = "RGBA8"; // Texture format, required.
	bool vs_texmip = false; // Auto-generate mipmaps, optional (defaults to false).
	bool vs_texrtt = true; // Texture is a target, optional (defaults to true).
>;
sampler defSampler = sampler_state { Texture = <defTexture>; };

// Simple invert shader:
float4 invert(in float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(mainSampler, uv);
	color.rgb = 1 - color.rgb;
	return color;
}

// Stripe shader:
float4 stripe(in float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(mainSampler, uv);
	color.r *= sin(uv.x * 20);
	color.g *= sin(uv.y * 20);
	return color;
}

// Combine shader:
float4 combine(in float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = 1;
	if (uv.x < 0.5) color = 	tex2D(mainSampler, uv);
	else		color = tex2D(defSampler, uv);
	return color;
}

technique main
{
	pass pDeferred
		< string target0 = "demo_defTexture"; >
	{
		PixelShader = compile ps_2_0 stripe();
	}
	pass pInvert
	{
		PixelShader = compile ps_2_0 invert();
	}
	pass pCombine
	{
		PixelShader = compile ps_2_0 combine();
	}
}