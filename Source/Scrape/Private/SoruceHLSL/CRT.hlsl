// Get scene UVs and screen resolution
float2 UV = GetDefaultSceneTextureUV(Parameters, 14);
float2 Size = ResolvedView.ViewSizeAndInvSize.xy;
float noise = saturate(Noise);

// Pixelation: LowFi controls pixel size
float Pixelation = lerp(1.0, 6.0, LowFi);
UV = floor(UV * Size / Pixelation) / (Size / Pixelation);

// Barrel distortion: bend edges outward
float2 Center = UV - 0.5;
UV = Center * (1.0 + dot(Center, Center) * (0.08 + noise * 0.25)) + 0.5;

// Horizontal screen tearing
// random nosie
float Horizontal = frac(sin(dot(float2(floor(uv.y * 40.0), floor(Time * 8.0)),
    float2(12.9898, 78.233))) * 43758.5453);
//shift
UV.x += step(0.985 - noise * 0.12, Horizontal) * (Horizontal - 0.5) * (0.04 + noise * 0.1);

//separate RGB channels
float ChromaticA = 0.0015 + noise * 0.012;
float3 Color = float3(
    SceneTextureLookup(uv + float2(ChromaticA, 0), 14, false).r,
    SceneTextureLookup(uv, 14, false).g,
    SceneTextureLookup(uv - float2(ChromaticA, 0), 14, false).b
);

// Scanlines:subtle horizontal brightness variation
Color *= 0.92 + 0.08 * sin(uv.y * Size.y * 3.14159 + Time * 5.0);

// Block glitches:randomly swap and brighten color channels
//random
float Block = frac(sin(dot(floor(UV * float2(24, 14)) + floor(Time * 10.0),
    float2(41.3, 91.7))) * 9999.0);
//blend
Color = lerp(Color, Color.bgr * 2.0,step(1.0 - saturate((Noise - 0.6) * 0.5), Center) * 0.6);

// Static noise: blend grayscale noise into the image
float Grain = frac(sin(dot(uv * Size + Time, float2(12.9898, 78.233))) * 43758.5453);
Color = lerp(Color, Grain.xxx, saturate((Noise - 0.85) * 6.67));

// Vignette: darken the screen corners
return Color * (1.0 - dot(Center, Center) * (0.1 + noise));


/*
// Get scene UVs and screen resolution
float2 uv = GetDefaultSceneTextureUV(Parameters, 14);
float2 size = ResolvedView.ViewSizeAndInvSize.xy;
float n = saturate(Noise);

// Pixelation: LowFi controls pixel size
float p = lerp(1.0, 6.0, LowFi);
uv = floor(uv * size / p) / (size / p);

// Barrel distortion: bend edges outward
float2 c = uv - 0.5;
uv = c * (1.0 + dot(c, c) * (0.08 + n * 0.25)) + 0.5;

// Horizontal screen tearing
// random nosie
float h = frac(sin(dot(float2(floor(uv.y * 40.0), floor(Time * 8.0)),
    float2(12.9898, 78.233))) * 43758.5453);
//shift
uv.x += step(0.985 - n * 0.12, h) * (h - 0.5) * (0.04 + n * 0.1);

//separate RGB channels
float ca = 0.0015 + n * 0.012;
float3 col = float3(
    SceneTextureLookup(uv + float2(ca, 0), 14, false).r,
    SceneTextureLookup(uv, 14, false).g,
    SceneTextureLookup(uv - float2(ca, 0), 14, false).b
);

// Scanlines:subtle horizontal brightness variation
col *= 0.92 + 0.08 * sin(uv.y * size.y * 3.14159 + Time * 5.0);

// Block glitches:randomly swap and brighten color channels
//random
float b = frac(sin(dot(floor(uv * float2(24, 14)) + floor(Time * 10.0),
    float2(41.3, 91.7))) * 9999.0);
//blend
col = lerp(col, col.bgr * 2.0,
    step(1.0 - saturate((n - 0.6) * 0.5), b) * 0.6);

// Static noise: blend grayscale noise into the image
float g = frac(sin(dot(uv * size + Time, float2(12.9898, 78.233))) * 43758.5453);
col = lerp(col, g.xxx, saturate((n - 0.85) * 6.67));

// Vignette: darken the screen corners
return col * (1.0 - dot(c, c) * (0.1 + n));
 */