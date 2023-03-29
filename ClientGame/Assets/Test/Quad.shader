Shader "Custom/Quad"
{
    Properties
    {
        _MainTex("BaseTexture", 2D) = "white" {}
        _DisolveTex("DisolveTex", 2D) = "white" {}
        _Color("Color", Color) = (1,1,1,1)

        // �ܺο��� �����Ҷ� _DisolveRatio�̰Ÿ� ��������.
        _DisolveRatio("DisolveRatio", Float) = 1

        
    }
    SubShader
    {
        Tags {"Queue" = "Transparent" "RenderType" = "Transparent" }
        
        // ���ĺ��� ����
        Blend SrcAlpha OneMinusSrcAlpha
        Pass
        {
            HLSLPROGRAM
            // ���̴� �� �����ϱ�
            #pragma target 4.5

            // ���ؽ� ���̴� ���
            #pragma vertex vert
            #pragma fragment frag

            // �ʿ��� �Լ��� ��߉�
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
                // SV_POSITION�� ���������� ȣȯ�� ������
                // Ŭ���� ���� ��ǥ�� ��Ÿ���µ� ����ϰ� 
                // ���ؽ� ���̴��� ��°��̵Ǿ� �ȼ����̴��� ���޵ɶ� ����.
                // 
                // �Ʒ��� ã�ƺ� ����
                //  
                // SV_POSITION�� ���� �� ����մϴ�
                // �̰��� ��� �÷������� �۵��մϴ�.
                // "POSITION"�� "��κ�"������ �۵��մϴ�.
                // ���� ��� PS4�� ���̴����� SV_POSITION�� �ʿ��ϴٰ� �����մϴ� .

                float4 position : SV_POSITION;
                float2 uv : TEXCOORD0;
            };

            Vertex vert(Attributes input)
            {
                Vertex output = (Vertex)0;

                // ���� �����̽�
                float4 positionOS = input.position;

                // ���� �����̽�
                float3 positionWS = TransformObjectToWorld(positionOS.xyz);

                // �� �����̽�
                float3 positionVS = TransformWorldToView(positionWS);

                // Ŭ���� �����̽�
                float4 positionCS = TransformWViewToHClip(positionVS);

                // ���� �����̽����� �� ������?

                output.position = positionCS;
                output.uv = input.uv;
                return output;
            }

            float4 frag(Vertex input) : SV_Target
            {
                float4 mainColor = SAMPLE_TEXTURE2D(_MainTex, sampler_MainTex, input.uv.xy);
                
                float4 disolveColor = SAMPLE_TEXTURE2D(_DisolveTex, sampler_DisolveTex, input.uv.xy);

                float alpha = (disolveColor.x + disolveColor.y + disolveColor.z) / 3;

                // ũ�⸦ -1 ~ 1������ �ش�.
                alpha = alpha + (2 * _DisolveRatio) - 1;

                mainColor.a = clamp(alpha, 0, 1);

                return mainColor;
            }

            ENDHLSL
        }
        
    }
}