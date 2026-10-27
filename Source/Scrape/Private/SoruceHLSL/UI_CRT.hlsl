float n = saturate(Noise);
float2 uv = UV;

// scan line
float scan = 0.06 * (0.5 + 0.5 * sin(uv.y * 900.0 + Time * 5.0));

// vingette
float2 c = uv - 0.5;
float vig = saturate(dot(c, c) * (1.6 + n * 2.0));

// grains
float g = frac(sin(dot(floor(uv * 400.0) + floor(Time * 24.0), float2(12.9898, 78.233))) * 43758.5453);
float grain = g * saturate((n - 0.3) * 0.8);

// tape banner
float row = floor(uv.y * 30.0);
float hh = frac(sin(dot(float2(row, floor(Time * 8.0)), float2(12.9898, 78.233))) * 43758.5453);
float band = step(0.985 - n * 0.12, hh) * 0.25;

// block noice >0.6
float2 blk = floor(uv * float2(24.0, 14.0));
float bn = frac(sin(dot(blk + floor(Time * 10.0), float2(41.3, 91.7))) * 9999.0);
float block = step(1.0 - saturate((n - 0.6) * 0.5), bn) * 0.35;

float a = saturate(scan + vig * 0.7 + grain + band + block);
float3 col = float3(g, g, g) * (grain + block) + float3(0, 0, 0);
return float4(col, a);