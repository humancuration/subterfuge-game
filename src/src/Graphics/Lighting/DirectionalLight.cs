using Godot;
using System;

public class DirectionalLight : Light
{
    public override void _Ready()
    {
        this.ShadowEnabled = true;
        this.ShadowMode = Light.ShadowModeEnum.VSM;
        this.ShadowAtlasSize = 2048;
    }

    // Add methods to toggle shadows based on game state if needed
}