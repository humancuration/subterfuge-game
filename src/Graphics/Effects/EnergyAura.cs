using Godot;
using System;

public class EnergyAura : MeshInstance
{
    private ShaderMaterial auraMaterial;
    private float time = 0.0f;

    public override void _Ready()
    {
        auraMaterial = (ShaderMaterial)MaterialOverride;
    }

    public override void _Process(float delta)
    {
        time += delta;
        auraMaterial.SetShaderParam("time", time);
    }

    public void ActivateAura(bool activate)
    {
        Visible = activate;
    }
}