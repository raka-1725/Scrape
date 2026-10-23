float2 position = (UV - 0.5) * 3.0;
float acc = 0.0;
float2 off = float2(0.8 + 0.1 * sin(Seed), 0.7 + 0.1 * cos(Seed * 1.7));

for (int i = 0; i < 8; i++)
{
	position = abs(position) / max(dot(position, position), 0.0001) - off;
	acc += exp(-length(position) * 2.5);
}

float phase = acc * 0.12 + Time * 0.03 + Seed;
float3 col = 0.5 + 0.5 * cos(6.2831 * (phase + float3(0.0, 0.33, 0.67)));
return color * saturate(acc * 0.35);









/*
float2 p = (UV - 0.5) * 3.0;
float acc = 0.0;
float2 off = float2(0.8 + 0.1 * sin(Seed), 0.7 + 0.1 * cos(Seed * 1.7));

for (int i = 0; i < 8; i++)
{
	p = abs(p) / max(dot(p, p), 0.0001) - off;
	acc += exp(-length(p) * 2.5);
}

float h = acc * 0.12 + Time * 0.03 + Seed;
float3 col = 0.5 + 0.5 * cos(6.2831 * (h + float3(0.0, 0.33, 0.67)));
return col * saturate(acc * 0.35);
 */