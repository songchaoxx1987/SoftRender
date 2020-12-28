#include "Vertex.h"
#include "define.h"

#define BARCENTRIC_LERP(a,b,c,p1,p2,p3) (a * p1 + b * p2 + c * p3);

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

void Vertex::BarycentricLerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, float p1, float p2, float p3)
{
//#define LERP(a) (pTrangle->v[0].a * lamda1 + pTrangle->v[1].a * lamda2 + pTrangle->v[2].a * lamda3) * z;
	float z = position.z;
	uv.x = z * BARCENTRIC_LERP(v1.uv.x, v2.uv.x, v3.uv.x, p1, p2, p3);
	uv.y = z * BARCENTRIC_LERP(v1.uv.y, v2.uv.y, v3.uv.y, p1, p2, p3);

	normal.x = z * BARCENTRIC_LERP(v1.normal.x, v2.normal.x, v3.normal.x, p1, p2, p3);
	normal.y = z * BARCENTRIC_LERP(v1.normal.y, v2.normal.y, v3.normal.y, p1, p2, p3);
	normal.z = z * BARCENTRIC_LERP(v1.normal.z, v2.normal.z, v3.normal.z, p1, p2, p3);

	worldPos.x = z * BARCENTRIC_LERP(v1.worldPos.x, v2.worldPos.x, v3.worldPos.x, p1, p2, p3);
	worldPos.y = z * BARCENTRIC_LERP(v1.worldPos.y, v2.worldPos.y, v3.worldPos.y, p1, p2, p3);
	worldPos.z = z * BARCENTRIC_LERP(v1.worldPos.z, v2.worldPos.z, v3.worldPos.z, p1, p2, p3);

	worldNormal.x = z * BARCENTRIC_LERP(v1.worldNormal.x, v2.worldNormal.x, v3.worldNormal.x, p1, p2, p3);
	worldNormal.y = z * BARCENTRIC_LERP(v1.worldNormal.y, v2.worldNormal.y, v3.worldNormal.y, p1, p2, p3);
	worldNormal.z = z * BARCENTRIC_LERP(v1.worldNormal.z, v2.worldNormal.z, v3.worldNormal.z, p1, p2, p3);

	normal.Normalize();
	worldNormal.Normalize();	
}