#include "Vertex.h"
#include "define.h"

Vertex Vertex::Lerp(const Vertex& v1, const Vertex& v2, float t)
{
	Vertex ret;
	ret.position = Vector3::Lerp(v1.position, v2.position, t);
	ret.worldPos = Vector3::Lerp(v1.worldPos, v2.worldPos, t);
	ret.normal = Vector3::Lerp(v1.normal, v2.normal, t);
	ret.worldNormal = Vector3::Lerp(v1.worldNormal, v2.worldNormal, t);
	ret.uv = Vector2::Lerp(v1.uv, v2.uv, t);
	ret.rhw = lerp(v1.rhw, v2.rhw, t);
	ret.color = lerp(v1.color, v2.color, t);
	return ret;
}