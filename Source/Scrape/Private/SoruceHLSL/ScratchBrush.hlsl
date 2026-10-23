//store inputs
float3 Holes[4] = { H0.xyz, H1.xyz, H2.xyz, H3.xyz };

//init output
float Strength = 0.0;

//loop all 4 output
for (int i = 0; i < 4; i++)
{
	//radius
	float radius = Holes[i].z;
	if (radius <= 0.0) continue;
	//get offset
	float2 distance = (UV - Holes[i].xy) * WallSize.xy; 
	//angle around center
	float ang = atan2(distance.y, distance.x);
    
	//generate wavy shape
	float wobble = 1.0
		+ 0.18 * sin(ang * 5.0  + Seed + i)
		+ 0.10 * sin(ang * 11.0 + Seed * 2.3)
		+ 0.05 * sin(ang * 23.0 + Seed * 4.1);
	//normilized dist
	float Normalizeddist = length(distance) / (radius * wobble);
	// dist to soft mask
	Strength = max(Strength, 1.0 - smoothstep(Hardness, 1.0, Normalizeddist));
}
return Strength;




/*
//store inputs
float3 H[4] = { H0.xyz, H1.xyz, H2.xyz, H3.xyz };

//init output
float s = 0.0;

//loop all 4 output
for (int i = 0; i < 4; i++)
{
	//radius
	float r = H[i].z;
	if (r <= 0.0) continue;
	//get offset
	float2 d = (UV - H[i].xy) * WallSize.xy; 
	//angle around center
	float ang = atan2(d.y, d.x);
    
	//generate wavy shape
	float wobble = 1.0
		+ 0.18 * sin(ang * 5.0  + Seed + i)
		+ 0.10 * sin(ang * 11.0 + Seed * 2.3)
		+ 0.05 * sin(ang * 23.0 + Seed * 4.1);
	//normilized dist
	float dist = length(d) / (r * wobble);
	// dist to soft mask
	s = max(s, 1.0 - smoothstep(Hardness, 1.0, dist));
}
return s;
*/