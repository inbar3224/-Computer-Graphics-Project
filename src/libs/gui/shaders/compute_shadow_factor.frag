const vec2 poisson64[] = vec2[](
	vec2(0.0617981, 0.07294159),
	vec2(0.6470215, 0.7474022),
	vec2(-0.5987766, -0.7512833),
	vec2(-0.693034, 0.6913887),
	vec2(0.6987045, -0.6843052),
	vec2(-0.9402866, 0.04474335),
	vec2(0.8934509, 0.07369385),
	vec2(0.1592735, -0.9686295),
	vec2(-0.05664673, 0.995282),
	vec2(-0.1203411, -0.1301079),
	vec2(0.1741608, -0.1682285),
	vec2(-0.09369049, 0.3196758),
	vec2(0.185363, 0.3213367),
	vec2(-0.1493771, -0.3147511),
	vec2(0.4452095, 0.2580113),
	vec2(-0.1080467, -0.5329178),
	vec2(0.1604507, 0.5460774),
	vec2(-0.4037193, -0.2611179),
	vec2(0.5947998, -0.2146744),
	vec2(0.3276062, 0.9244621),
	vec2(-0.6518704, -0.2503952),
	vec2(-0.3580975, 0.2806469),
	vec2(0.8587891, 0.4838005),
	vec2(-0.1596546, -0.8791054),
	vec2(-0.3096867, 0.5588146),
	vec2(-0.5128918, 0.1448544),
	vec2(0.8581337, -0.424046),
	vec2(0.1562584, -0.5610626),
	vec2(-0.7647934, 0.2709858),
	vec2(-0.3090832, 0.9020988),
	vec2(0.3935608, 0.4609676),
	vec2(0.3929337, -0.5010948),
	vec2(-0.8682281, -0.1990303),
	vec2(-0.01973724, 0.6478714),
	vec2(-0.3897587, -0.4665619),
	vec2(-0.7416366, -0.4377831),
	vec2(-0.5523247, 0.4272514),
	vec2(-0.5325066, 0.8410385),
	vec2(0.3085465, -0.7842533),
	vec2(0.8400612, -0.200119),
	vec2(0.6632416, 0.3067062),
	vec2(-0.4462856, -0.04265022),
	vec2(0.06892014, 0.812484),
	vec2(0.5149567, -0.7502338),
	vec2(0.6464897, -0.4666451),
	vec2(-0.159861, 0.1038342),
	vec2(0.6455986, 0.04419327),
	vec2(-0.7445076, 0.5035095),
	vec2(0.9430245, 0.3139912),
	vec2(0.0349884, -0.7968109),
	vec2(-0.9517487, 0.2963554),
	vec2(-0.7304786, -0.01006928),
	vec2(-0.5862702, -0.5531025),
	vec2(0.3029106, 0.09497032),
	vec2(0.09025345, -0.3503742),
	vec2(0.4356628, -0.0710125),
	vec2(0.4112572, 0.7500054),
	vec2(0.3401214, -0.3047142),
	vec2(-0.2192158, -0.6911137),
	vec2(-0.4676369, 0.6570358),
	vec2(0.6295372, 0.5629555),
	vec2(0.1253822, 0.9892166),
	vec2(-0.1154335, 0.8248222),
	vec2(-0.4230408, -0.7129914)
);

uniform sampler2DShadow shadowMap;
uniform sampler2D shadowDepth;
uniform float bias;
uniform int PCF_mode; // 0-normal, 1-variable grid, 2-weighted grid, 3-poisson
uniform int PCF_samples_num;
uniform float shadow_spread;
uniform float light_size = 10;



float CalcShadowFactor(vec4 LightSpacePos)
{
	LightSpacePos.xyz = 0.5*LightSpacePos.xyz+0.5;
	float visibility=textureProj(shadowMap, vec4(LightSpacePos.xy,  LightSpacePos.z-bias,LightSpacePos.w));

	return visibility;
}

float CalcShadowFactorSoft(vec4 LightSpacePos)
{
	float vis = 0.0;
	int n = PCF_samples_num;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++){
			vec4 p = LightSpacePos + vec4(shadow_spread * (i - n/2), shadow_spread * (j - n/2), 0.0, 0.0);
			vis = vis + CalcShadowFactor(p);
		}
	}
	return (1.0/(n*n))*vis;
}


float CalcShadowFactorWeighted(vec4 LightSpacePos)
{
	float vis = 0.0;
	float weights[5] = float[5](0.1, 0.2, 0.4, 0.2, 0.1);
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++){
			vec4 p = LightSpacePos + vec4(shadow_spread * (i - 2), shadow_spread * (j - 2), 0.0, 0.0);
			vis = vis + weights[i] * weights[j] * CalcShadowFactor(p);
		}
	}
	return vis;
}

float CalcShadowFactorPoisson(vec4 LightSpacePos)
{
	float vis = 0.0;
	for(int i = 0; i < PCF_samples_num; i++)
		{
			vec4 p = LightSpacePos - vec4(2*shadow_spread * poisson64[i].x, 2*shadow_spread * poisson64[i].y, 0.0, 0.0);
			vis = vis + CalcShadowFactor(p)/PCF_samples_num;
		}
	return vis;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float CalcShadowFactorPoissonStratified(vec4 LightSpacePos)
{
	float vis = 0.0;
	int i = int(64.0*rand(gl_FragCoord.xy))%64;
	vec4 p = LightSpacePos + vec4(2*shadow_spread * poisson64[i].x, 2*shadow_spread * poisson64[i].y, 0.0, 0.0);
	vis = CalcShadowFactor(p);

	return vis;
}


float CalcShadowFactorPoissonRotated(vec4 LightSpacePos)
{
	float vis = 0.0;
	for(int i = 0; i < PCF_samples_num; i++)
	{
		// create a random rotation. To do: change to random texture based https://andrew-pham.blog/2019/08/03/percentage-closer-soft-shadows/
		float r = rand(gl_FragCoord.xy);
		vec2 offset = mat2(r,1-r,r-1,r)*vec2(poisson64[i]);
		vec4 p = LightSpacePos - vec4(2*shadow_spread * offset.x , 2*shadow_spread * offset.y, 0.0, 0.0);
		vis = vis + CalcShadowFactor(p)/PCF_samples_num;
	}
	return vis;
}


 float calcSearchWidth(float receiverDepth)
{
    return light_size;// * (receiverDepth - NEAR) / gViewPos.z;
}


float calcBlockerDistance(vec4 LightSpacePos, float bias)
{
    float sumBlockerDistances = 0.0f;
    int numBlockerDistances = 0;
	vec3 LightSpacePosTex = 0.5*LightSpacePos.xyz+0.5;
    float receiverDepth = LightSpacePosTex.z/LightSpacePos.w;
 
    float sw = calcSearchWidth(receiverDepth);
	
	float depth;
	float dist;
	for(int i = 0; i < 16; i++)
    {
		float r = rand(gl_FragCoord.xy);
		vec2 offset = mat2(r,1-r,r-1,r)*vec2(poisson64[i]);
        depth = textureProj(shadowDepth, vec3(LightSpacePosTex.xy+shadow_spread*offset,  LightSpacePos.w)).r;
		dist = receiverDepth - depth;

        if (dist > bias)
        {
            numBlockerDistances++;
            sumBlockerDistances += dist;
        }
    }
 
    if (numBlockerDistances > 0)
    {
        return sumBlockerDistances / numBlockerDistances;
    }
    else
    {
        return -1;
    }
}

float calcPCFKernelSize(vec4 LightSpacePos, float bias)
{
   	float blockerDistance = calcBlockerDistance(LightSpacePos,bias);

	vec3 LightSpacePosTex = 0.5*LightSpacePos.xyz+0.5;
    float receiverDepth = LightSpacePosTex.z/LightSpacePos.w;
	
    if (blockerDistance == -1)
    {
        return 0;
    }
    float penumbraWidth = blockerDistance;
    return penumbraWidth; // * gCalibratedLightSize * NEAR / receiverDepth;
}

float CalcShadowFactorPoissonRotatedAdaptivePCF(vec4 LightSpacePos)
{
	float vis = 0.0;
	float pcfKernelSize = 100*calcPCFKernelSize(LightSpacePos, bias);
	//vis=pcfKernelSize;
	vec3 LightSpacePosTex = 0.5*LightSpacePos.xyz+0.5;
	for(int i = 0; i < PCF_samples_num; i++)
	{
		// create a random rotation. To do: change to random texture based https://andrew-pham.blog/2019/08/03/percentage-closer-soft-shadows/
		float r = rand(gl_FragCoord.xy);
		vec2 offset = mat2(r,1-r,r-1,r)*vec2(poisson64[i]);
		vec4 p = LightSpacePos - vec4(2*pcfKernelSize*shadow_spread * offset.x , 2*pcfKernelSize*shadow_spread * offset.y, 0.0, 0.0);
		vis = vis + CalcShadowFactor(p)/PCF_samples_num;
	}
	return vis;
}



float computeShadowMapShading(){

	float shadowFactor;
	if(PCF_mode==0)
		shadowFactor = CalcShadowFactor(lightSpacePos);
	if(PCF_mode==1)
		shadowFactor = CalcShadowFactorSoft(lightSpacePos);
	if(PCF_mode==2)
		shadowFactor = CalcShadowFactorWeighted(lightSpacePos);
	if(PCF_mode==3)
		shadowFactor = CalcShadowFactorPoisson(lightSpacePos);
	if(PCF_mode==4)
		shadowFactor = CalcShadowFactorPoissonStratified(lightSpacePos);
	if(PCF_mode==5)
		shadowFactor = CalcShadowFactorPoissonRotated(lightSpacePos);
	if(PCF_mode==6)
		shadowFactor = CalcShadowFactorPoissonRotatedAdaptivePCF(lightSpacePos);

	return shadowFactor;
}