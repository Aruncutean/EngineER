#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform mat4 inverseViewProjection;
uniform vec3 cameraPos;
uniform vec3 sunDirection;

in vec3 vPosition;

#define PI 3.141592
#define iSteps 16
#define jSteps 8
#define planetRadius 6372e3

vec2 rsi(vec3 r0, vec3 rd, float sr) {
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b * b) - 4.0 * a * c;
    if(d < 0.0)
        return vec2(1e5, -1e5);
    return vec2((-b - sqrt(d)) / (2.0 * a), (-b + sqrt(d)) / (2.0 * a));
}

vec3 atmosphere(vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
    pSun = normalize(pSun);
    r = normalize(r);

    vec2 p = rsi(r0, r, rAtmos);
    if(p.x > p.y)
        return vec3(0, 0, 0);
    p.y = min(p.y, rsi(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    float iTime = 0.0;

    vec3 totalRlh = vec3(0, 0, 0);
    vec3 totalMie = vec3(0, 0, 0);

    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    for(int i = 0; i < iSteps; i++) {

        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        float iHeight = length(iPos) - rPlanet;

        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

        float jTime = 0.0;

        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        for(int j = 0; j < jSteps; j++) {

            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            float jHeight = length(jPos) - rPlanet;

            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            jTime += jStepSize;
        }

        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        iTime += iStepSize;

    }

    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

void main() {

    vec4 clipPos = vec4(TexCoords * 2.0 - 1.0, 1.0, 1.0);
    vec4 worldPos = inverseViewProjection * clipPos;
    worldPos /= worldPos.w;
    vec3 rayDir = normalize(worldPos.xyz - cameraPos);

    vec3 col = atmosphere(rayDir, vec3(0, planetRadius, 0) + cameraPos, sunDirection, 22.0, planetRadius, planetRadius + 100e3, vec3(5.5e-6, 13.0e-6, 22.4e-6), 21e-6, 8e3, 1.2e3, 0.758);

    col = 1.0 - exp(-1.0 * col);

    FragColor = vec4(col, 1);
}