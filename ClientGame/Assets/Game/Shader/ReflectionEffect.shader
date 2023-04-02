Shader "Custom/ReflectionEffect"
{
    Properties
    {
        _BaseTexture("BaseTexture", 2D) = "white" {}
        _ReciveDirUV("ReciveDirUV", Vector) = (1,0,1,1)
    }
        SubShader
        {
            Tags {"Queue" = "Transparent" "RenderType" = "Transparent" }

            Cull Off


            // 알파블렌드 세팅
            Blend SrcAlpha OneMinusSrcAlpha
            Pass
            {
                HLSLPROGRAM
                // 쉐이더 모델 지정하기
                #pragma target 4.5

                // 버텍스 쉐이더 사용
                #pragma vertex vert
                #pragma fragment frag

                // 필요한 함수들 써야됌
                #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"

                TEXTURE2D(_BaseTexture); SAMPLER(sampler_BaseTexture);
                float3 _ReciveDirUV;

                struct Attributes
                {
                    float4 position : POSITION;
                    float2 uv : TEXCOORD0;
                };

                struct Vertex
                {
                    // SV(System-Value)
                    // SV_POSITION을 쓰는이유는 호환성 때문에
                    // 클리핑 공간 좌표를 나타내는데 사용하고 
                    // 버텍스 쉐이더의 출력값이되어 픽셀쉐이더에 전달될때 쓴다.
                    // 
                    // 아래는 찾아본 내용
                    //  
                    // SV_POSITION을 가장 잘 사용합니다
                    // 이것은 모든 플랫폼에서 작동합니다.
                    // "POSITION"은 "대부분"에서만 작동합니다.
                    // 예를 들어 PS4의 셰이더에는 SV_POSITION이 필요하다고 생각합니다 .

                    float4 position : SV_POSITION;
                    float2 uv : TEXCOORD0;
                };

                Vertex vert(Attributes input)
                {
                    Vertex output = (Vertex)0;

                    // 로컬 스페이스
                    float4 positionOS = input.position;

                    // 월드 스페이스
                    float3 positionWS = TransformObjectToWorld(positionOS.xyz);

                    // 뷰 스페이스
                    float3 positionVS = TransformWorldToView(positionWS);

                    // 클리핑 스페이스
                    float4 positionCS = TransformWViewToHClip(positionVS);

                    // 투영 스페이스까지 왜 안하지?

                    output.position = positionCS;
                    output.uv = input.uv;
                    return output;
                }

                float4 frag(Vertex input) : SV_Target
                {
                    float4 mainColor = SAMPLE_TEXTURE2D(_BaseTexture, sampler_BaseTexture, input.uv.xy);

                    // 계산하기 쉽게 좌표기준을 바꾸자
                    //float2 x = input.uv.x * 2 - 1;
                    //float2 y = input.uv.y * 2 - 1;

                    if (_ReciveDirUV.z == 0)
                    {
                        mainColor.a *= 0.6;
                        return mainColor;
                    }
                    float x = abs(_ReciveDirUV.x - input.uv.x);
                    float y = abs(_ReciveDirUV.y - input.uv.y);

                    float distance = sqrt(x * x + y * y);

                    if (distance < 0.5)
                    {
                        float alphaRatio = distance / 0.5;

                        mainColor.r *= 1 - 0.9 * (1 - alphaRatio);

                        mainColor.a *= 0.6 + 1.5 * (1 - alphaRatio);
                    }
                    else
                        mainColor.a *= 0.6;

                    return mainColor;
                }

                ENDHLSL
            }
        }
}
