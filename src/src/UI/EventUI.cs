using Godot;
using Systems;
using Models;

public class EventUI : Control
{
    private Label titleLabel;
    private RichTextLabel descriptionLabel;
    private VBoxContainer choicesContainer;
    private ComplexEventSystem complexEventSystem;

    public override void _Ready()
    {
        titleLabel = GetNode<Label>("TitleLabel");
        descriptionLabel = GetNode<RichTextLabel>("DescriptionLabel");
        choicesContainer = GetNode<VBoxContainer>("ChoicesContainer");

        // Get ComplexEventSystem from GameManager
        complexEventSystem = GetParent().GetNode<GameManager>("GameManager").GetComplexEventSystem();
        complexEventSystem.ConnectEventNotifier(this);

        // Hide the UI initially
        Visible = false;
    }

    public void DisplayEvent(ComplexGameEvent gameEvent)
    {
        titleLabel.Text = gameEvent.TitleKey; // Assuming localization is handled in ComplexEventSystem
        descriptionLabel.Text = gameEvent.Description;

        // Clear previous choices
        foreach (Node child in choicesContainer.GetChildren())
        {
            child.QueueFree();
        }

        foreach (var choice in gameEvent.Choices)
        {
            Button choiceButton = new Button
            {
                Text = choice.ChoiceText
            };
            choiceButton.Connect("pressed", this, nameof(OnChoiceSelected), new Godot.Collections.Array { gameEvent, choice });
            choicesContainer.AddChild(choiceButton);
        }

        Visible = true;
    }

    private void OnChoiceSelected(ComplexGameEvent gameEvent, PlayerChoice choice)
    {
        complexEventSystem.HandlePlayerChoice(gameEvent, choice);
        // Hide the UI after making a choice
        Visible = false;
    }
}