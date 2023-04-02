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
                    float4 mainColor = SAMPLE_TEXTURE2D(_BaseTexture, sampler_BaseTexture, input.uv.xy);

                    // ����ϱ� ���� ��ǥ������ �ٲ���
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
