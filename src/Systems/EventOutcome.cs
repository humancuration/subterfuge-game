using System.Collections.Generic;

namespace Systems
{
    public class EventOutcome
    {
        public Dictionary<string, float> Effects { get; set; }
        public List<string> TriggerNextEvents { get; set; }

        public EventOutcome()
        {
            Effects = new Dictionary<string, float>();
            TriggerNextEvents = new List<string>();
        }
    }
}