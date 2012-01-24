
// Voodoo Shader
// Demo Effect: NL Bloom
// This effect demonstrates the use of parameter-created textures for deferred rendering.

Texture2D frameTex < string vs_texture = ":frame0"; >;
sampler frameSampler = sampler_state { Texture = <frameTex>; };

// Link to the last pass:
Texture2D mainTexture < string vs_texture = ":pass0"; >;
sampler mainSampler = sampler_state 
{ 
	Texture = <mainTexture>;
	AddressU = Clamp; 
	AddressV = Clamp; 
	MinFilter = GaussianQuad; 
	MagFilter = GaussianQuad; 
};

// Create a new texture:
Texture2D smallTexture
<
	string vs_texture = "nlbloom_small";
	bool vs_texcreate = true; // Texture creation flag, must be true for a new texture.
	int2 vs_texdim = {256, 256}; // Texture size, required.
	string vs_texfmt = "RGBA8"; // Texture format, required.
	bool vs_texmip = false; // Auto-generate mipmaps, optional (defaults to false).
	bool vs_texrtt = true; // Texture is a target, optional (defaults to true).
>;
sampler smallSampler = sampler_state { Texture = <smallTexture>; };

// Globals:
float2 rcpres < string source = "rcpres"; > = {0.02,0.02};

// Blur kernels:
float3 blurVertical[] = 
{
    { 0, -3, 2 },
	{ 0, -2, 3 },
	{ 0, -1, 4 },
	{ 0,  0, 5 },
	{ 0,  1, 4 },
	{ 0,  2, 3 },
	{ 0,  3, 2 }
};

float3 blurHorizontal[] =
{
    { -3, 0, 2 },
	{ -2, 0, 3 },
	{ -1, 0, 4 },
	{  0, 0, 5 },
	{  1, 0, 4 },
	{  2, 0, 3 },
	{  3, 0, 2 }
};

// Calculate luminance:
float luminance(in float3 rgb)
{
	return ((rgb.r * 0.2126) + (rgb.g * 0.7152) + (rgb.b * 0.0722));
}

float rcplum(in float3 rgb)
{
	return ((rgb.r / 0.2126) + (rgb.g / 0.7152) + (rgb.b / 0.0722));
}

// Downsize pass:
float4 downsize(in float2 uv : TEXCOORD0) : COLOR0
{
	return tex2D(mainSampler, uv*8);
}

// Bright pass:
float4 brightpass(in float2 uv : TEXCOORD0, uniform sampler samp) : COLOR0
{
	float4 color = tex2D(samp, uv);
	float lum = luminance(color.rgb);
	float lumAdj = 1 - abs(0.8 - log(lum * 2.6));
	lumAdj = clamp(0, 1, pow(lumAdj, 3));
	lumAdj = pow(lumAdj,5);
	float3 chanMult = cross(color.rgb, 1 - rcplum(color.rgb));
	color.rgb *= chanMult * lumAdj;
	return color;
}

// Blur pass:
float4 blur(in float2 uv : TEXCOORD0, uniform sampler samp, uniform float m, uniform int a, uniform int b, uniform float3 kernel[7]) : COLOR0
{
	float4 color = 0;
	float mult = 0;
	for (int i = a; i < b; ++i)
	{
		float2 tuv = uv + (kernel[i].xy * rcpres);
		float3 tap = tex2D(samp, tuv).rgb;
		color.rgb += tap * kernel[i].z;
		mult += kernel[i].z;
	}
	color.rgb = color.rgb / mult;
	return color;
}

// Combine shader:
float4 combine(in float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(frameSampler, uv);
	if (uv.x > 0.5) color += tex2D(mainSampler, uv);
	return color;
}

technique main
{
	/*pass pShrink
		//< string target0 = "nlbloom_small"; >
	{
		PixelShader = compile ps_2_0 downsize();
	}*/
	pass pBright
	{
		PixelShader = compile ps_2_0 brightpass(mainSampler);
	}
	pass pBlurH 
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurHorizontal);
	}
	pass pBlurV
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurVertical);
	}
	pass pBlurH 
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurHorizontal);
	}
	pass pBlurV
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurVertical);
	}
	pass pBlurH 
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurHorizontal);
	}
	pass pBlurV
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurVertical);
	}
	pass pBlurH 
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurHorizontal);
	}
	pass pBlurV
	{
		PixelShader = compile ps_2_0 blur(mainSampler, 1, 0, 7, blurVertical);
	}
	pass pCombine
	{
		PixelShader = compile ps_2_0 combine();
	}
}
