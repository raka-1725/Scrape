float2 uv = GetDefaultSceneTextureUV(Parameters, 14);
float2 px = ResolvedView.ViewSizeAndInvSize.zw;

//depth to world
float depth = CalcSceneDepth(uv);
float2 NormalizedDeviceCoordinate = float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0);
float4 clipPos = mul(float4(NormalizedDeviceCoordinate, 1.0, 1.0), ResolvedView.ClipToTranslatedWorld);
float3 Direction = normalize(clipPos.xyz / clipPos.w - ResolvedView.TranslatedWorldCameraOrigin);
float3 WorldSpacePosition = LWCToFloat(ResolvedView.WorldCameraOrigin) + Direction * (depth / dot(Direction, ResolvedView.ViewForward));

//depth edge
float DepthLeft = CalcSceneDepth(uv + float2(-px.x, 0));
float DepthRight = CalcSceneDepth(uv + float2( px.x, 0));
float DepthUp = CalcSceneDepth(uv + float2(0, -px.y));
float DepthDown = CalcSceneDepth(uv + float2(0,  px.y));
float DepthDiff = (abs(DepthLeft - depth) + abs(DepthRight - depth) + abs(DepthUp - depth) + abs(DepthDown - depth)) / (depth * 0.03 + 1.0);
float Edge = smoothstep(0.3, 1.0, saturate(DepthDiff));

//sonar sources
float4 Sonar[4] = { S0, S1, S2, S3 };
float  StrengthArr[4] = { Strength.x, Strength.y, Strength.z, Strength.w };
float ring = 0.0;
//calc ring for each source
for (int i = 0; i < 4; i++)
{
	float dist  = distance(WorldSpacePosition, Sonar[i].xyz);
	float front = 1.0 - saturate(abs(dist - Sonar[i].w) / Width);
	float trail = saturate((Sonar[i].w - dist) / Trail) * step(dist, Sonar[i].w);
	ring = max(ring, (front + trail * 0.35) * Strength[i]);
}
//color
float3 color = lerp(ColorA, ColorExit, step(0.5, ExitMask));
float3 sonar = ring * (Edge + 0.10) * color * 5.0;
sonar = lerp(sonar, ring * ColorExit * 2.0, step(0.5, ExitMask));

return SceneColor * 0.75 + sonar;





/*
float lum = max(SceneColor.r, max(SceneColor.g, SceneColor.b));
float3 glow = SceneColor * smoothstep(0.25, 0.6, lum);
return glow + sonar;
*/













/*
*float2 uv = GetDefaultSceneTextureUV(Parameters, 14);
float2 px = ResolvedView.ViewSizeAndInvSize.zw;

//depth to world
float d = CalcSceneDepth(uv);
float2 ndc = float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0);
float4 h = mul(float4(ndc, 1.0, 1.0), ResolvedView.ClipToTranslatedWorld);
float3 dir = normalize(h.xyz / h.w - ResolvedView.TranslatedWorldCameraOrigin);
float3 wp = LWCToFloat(ResolvedView.WorldCameraOrigin) + dir * (d / dot(dir, ResolvedView.ViewForward));

//depth edge
float dl = CalcSceneDepth(uv + float2(-px.x, 0));
float dr = CalcSceneDepth(uv + float2( px.x, 0));
float du = CalcSceneDepth(uv + float2(0, -px.y));
float dd = CalcSceneDepth(uv + float2(0,  px.y));
float e = (abs(dl - d) + abs(dr - d) + abs(du - d) + abs(dd - d)) / (d * 0.03 + 1.0);
float edge = smoothstep(0.3, 1.0, saturate(e));

//sonar sources
float4 S[4] = { S0, S1, S2, S3 };
float  St[4] = { Strength.x, Strength.y, Strength.z, Strength.w };
float ring = 0.0;
//calc ring for each source
for (int i = 0; i < 4; i++)
{
	float dist  = distance(wp, S[i].xyz);
	float front = 1.0 - saturate(abs(dist - S[i].w) / Width);
	float trail = saturate((S[i].w - dist) / Trail) * step(dist, S[i].w);
	ring = max(ring, (front + trail * 0.35) * St[i]);
}
//color
float3 col = lerp(ColorA, ColorExit, step(0.5, ExitMask));
float3 sonar = ring * (edge + 0.10) * col * 5.0;
sonar = lerp(sonar, ring * ColorExit * 2.0, step(0.5, ExitMask));

return SceneColor * 0.75 + sonar;





/*
float lum = max(SceneColor.r, max(SceneColor.g, SceneColor.b));
float3 glow = SceneColor * smoothstep(0.25, 0.6, lum);
return glow + sonar;
*/