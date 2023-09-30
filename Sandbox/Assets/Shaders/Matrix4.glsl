#version 430 core

// RH Lookat
mat4 lookat(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = (normalize(center - eye));
	vec3 s = (normalize(cross(f, up)));
	vec3 u = (cross(s, f));

	mat4 view = 
	{
		vec4(s.x, s.y, s.z, -dot(s, eye)),
		vec4(u.x, u.y, u.z, -dot(u, eye)),
		vec4(-f.x, -f.y, -f.z, dot(f, eye)),
		vec4(0, 0, 0, 1)
	};
	return view;
}