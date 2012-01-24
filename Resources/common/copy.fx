// Voodoo Shader test effect
// Copyright (c) 2010-2012 by Sean Sube, All Rights Reserved.
//
// This is a test shader. The effect is not meant to be pretty.
// There is significant code and comments to demonstrate Voodoo Shader features.


// Link to an existing texture:
Texture2D mainTex 
< 
	string vs_texture = ":pass0";	// Name of the texture to use. Pass 0 is the most recent pass. Passes and frames count up in reverse chronological order.
>;
sampler mainSamp  = sampler_state { Texture = <mainTex>; };

float4 copy(in float2 uv : TEXCOORD0) : COLOR0
{
    return tex2D(mainSamp, uv);
}

technique main 
{
    pass copy
    {
        PixelShader = compile ps_2_0 copy();
    }
}
