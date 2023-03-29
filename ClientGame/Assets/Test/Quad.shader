Shader "Custom/Quad"
{
    Properties
    {
        _MainTex("BaseTexture", 2D) = "white" {}
        _DisolveTex("DisolveTex", 2D) = "white" {}
        _Color("Color", Color) = (1,1,1,1)

        // 외부에서 접근할때 _DisolveRatio이거를 접근하자.
        _DisolveRatio("DisolveRatio", Float) = 1

        
    }
    SubShader
    {
        Tags {"Queue" = "Transparent" "RenderType" = "Transparent" }
        
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

            TEXTURE2D(_MainTex); SAMPLER(sampler_MainTex);
            TEXTURE2D(_DisolveTex); SAMPLER(sampler_DisolveTex);

            float _DisolveRatio;

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
                float4 mainColor = SAMPLE_TEXTURE2D(_MainTex, sampler_MainTex, input.uv.xy);
                
                float4 disolveColor = SAMPLE_TEXTURE2D(_DisolveTex, sampler_DisolveTex, input.uv.xy);

                float alpha = (disolveColor.x + disolveColor.y + disolveColor.z) / 3;

                // 크기를 -1 ~ 1까지는 준다.
                alpha = alpha + (2 * _DisolveRatio) - 1;

                mainColor.a = clamp(alpha, 0, 1);

                return mainColor;
            }

            ENDHLSL
        }
        
    }
}