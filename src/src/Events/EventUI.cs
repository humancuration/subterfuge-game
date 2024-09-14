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
        var gameManager = GetNode<"/root/GameManager"]<GameManager>();
        complexEventSystem = gameManager.GetComplexEventSystem();

        // Connect to event triggers
        // This depends on how ComplexEventSystem notifies the UI
        // For example, you might emit a signal when a new event is triggered
    }

    public void DisplayEvent(ComplexGameEvent gameEvent)
    {
        currentEvent = gameEvent;
        titleLabel.Text = gameEvent.Title;
        descriptionLabel.Text = gameEvent.Description;

        // Clear previous choices
        choicesContainer.QueueFreeChildren();

        foreach (var choice in gameEvent.Choices)
        {
            Button choiceButton = new Button
            {
                Text = choice.ChoiceText
            };
            choiceButton.Connect("pressed", this, nameof(OnChoiceSelected), new Godot.Collections.Array { choice });
            choicesContainer.AddChild(choiceButton);
        }

        Visible = true;
    }

    private void OnChoiceSelected(PlayerChoice choice)
    {
        if (currentEvent != null && choice != null)
        {
            complexEventSystem.HandlePlayerChoice(currentEvent, choice);
            currentEvent = null;
            Visible = false;
        }
    }
}

// o1 mini
// DisplayEvent: Populates the UI with event details and dynamically creates buttons for each player choice.
// OnChoiceSelected: Processes the player's choice by informing the ComplexEventSystem.