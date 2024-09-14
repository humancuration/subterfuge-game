// File: Models/DateTime.cs
// Time System
// Implement a day-night cycle and a calendar system to track the passage of time, influencing NPC behaviors, events, and the game world.

namespace Models
{
    public class DateTime
    {
        public int Day { get; set; }
        public int Month { get; set; }
        public int Year { get; set; }
        public float TimeOfDay { get; set; }

        public DateTime(int day, int month, int year, float timeOfDay)
        {
            Day = day;
            Month = month;
            Year = year;
            TimeOfDay = timeOfDay;
        }

        public void AdvanceTime(float deltaTime)
        {
            TimeOfDay += deltaTime;
            if (TimeOfDay >= 24f)
            {
                TimeOfDay -= 24f;
                Day++;
                if (Day > 30)
                {
                    Day = 1;
                    Month++;
                    if (Month > 12)
                    {
                        Month = 1;
                        Year++;
                    }
                }
            }
        }
    }
}
