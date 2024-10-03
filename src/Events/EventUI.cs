using Godot;
using Models;
using Systems;

public class EventUI : Control
{
    private Label titleLabel;
    private RichTextLabel descriptionLabel;
    private VBoxContainer choicesContainer;
    private ComplexEventSystem complexEventSystem;
    private ComplexGameEvent currentEvent;

    public override void _Ready()
    {
        titleLabel = GetNode<Label>("TitleLabel");
        descriptionLabel = GetNode<RichTextLabel>("DescriptionLabel");
        choicesContainer = GetNode<VBoxContainer>("ChoicesContainer");

        // Assume ComplexEventSystem is accessible via GameManager
        var gameManager = GetNode<GameManager>("/root/GameManager");
        complexEventSystem = gameManager.GetComplexEventSystem();

        // Connect to event triggers
        // This depends on how ComplexEventSystem notifies the UI
        // For example, you might emit a signal when a new event is triggered
    }

    // ... rest of the code remains unchanged ...
}
// o1 mini
// DisplayEvent: Populates the UI with event details and dynamically creates buttons for each player choice.
// OnChoiceSelected: Processes the player's choice by informing the ComplexEventSystem.