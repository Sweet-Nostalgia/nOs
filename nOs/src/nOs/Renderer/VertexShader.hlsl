struct VS_OUT {
    float4 Pos : SV_POSITION;
};

VS_OUT main(float3 pos : POSITION) {
    VS_OUT output;
    output.Pos = float4(pos, 1.0f);
    return output;
}