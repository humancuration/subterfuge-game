import { motion, AnimatePresence } from "framer-motion";
import { useState } from "react";
import { useFetcher } from "@remix-run/react";
import confetti from "canvas-confetti";

interface LearningMilestone {
  id: string;
  title: string;
  description: string;
  date: string;
  type: "insight" | "creation" | "collaboration" | "teaching" | "achievement";
  reflections?: string[];
  connections: string[]; // IDs of related milestones
  impact: {
    personal: string;
    community: string;
  };
}

const milestoneEmojis = {
  insight: "💡",
  creation: "🎨",
  collaboration: "🤝",
  teaching: "📚",
  achievement: "🌟"
};

const encouragingMessages = [
  "Every step is progress! 🌱",
  "Growth looks good on you! ✨",
  "You're on your own path! 🛣️",
  "Learning is the journey! 🚀",
  "Keep exploring! 🗺️",
  "Your curiosity is beautiful! 🦋"
];

export function LearningJourney() {
  const [selectedMilestone, setSelectedMilestone] = useState<LearningMilestone | null>(null);
  const [isReflecting, setIsReflecting] = useState(false);
  const fetcher = useFetcher();

  const handleAddReflection = (milestoneId: string, reflection: string) => {
    confetti({
      particleCount: 30,
      spread: 60,
      colors: ['#4CAF50', '#2196F3', '#9C27B0']
    });
    
    fetcher.submit(
      { milestoneId, reflection, intent: "addReflection" },
      { method: "post" }
    );
  };

  return (
    <div className="learning-journey p-6 bg-white rounded-xl shadow-lg">
      <div className="flex justify-between items-center mb-8">
        <div>
          <h2 className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-green-600 to-blue-600">
            Your Learning Journey 🌱
          </h2>
          <p className="text-gray-600 mt-1">
            {encouragingMessages[Math.floor(Math.random() * encouragingMessages.length)]}
          </p>
        </div>
        <motion.button
          whileHover={{ scale: 1.05 }}
          whileTap={{ scale: 0.95 }}
          onClick={() => setIsReflecting(true)}
          className="px-4 py-2 bg-gradient-to-r from-green-500 to-blue-500 text-white rounded-lg"
        >
          Add Reflection ✍️
        </motion.button>
      </div>

      {/* Journey Timeline */}
      <div className="relative">
        <div className="absolute left-1/2 transform -translate-x-1/2 h-full w-1 bg-gradient-to-b from-green-200 to-blue-200" />
        
        {/* Milestone nodes would map over actual data */}
        {[1, 2, 3].map((_, index) => (
          <motion.div
            key={index}
            initial={{ opacity: 0, x: index % 2 ? 20 : -20 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ delay: index * 0.2 }}
            className={`flex ${
              index % 2 ? "flex-row" : "flex-row-reverse"
            } items-center mb-12`}
          >
            <div className={`w-1/2 ${index % 2 ? "pr-8 text-right" : "pl-8"}`}>
              <motion.div
                whileHover={{ scale: 1.02 }}
                className="bg-gradient-to-br from-green-50 to-blue-50 p-4 rounded-lg shadow-sm"
              >
                <span className="text-2xl mb-2 inline-block">
                  {milestoneEmojis.insight}
                </span>
                <h3 className="font-bold text-lg">Learning Milestone {index + 1}</h3>
                <p className="text-gray-600 text-sm mt-1">
                  A meaningful step in your journey...
                </p>
                
                {/* Reflections */}
                <div className="mt-3 space-y-2">
                  <p className="text-sm italic text-gray-500">
                    "This helped me understand..."
                  </p>
                </div>

                {/* Impact */}
                <div className="mt-3 flex gap-2">
                  <span className="text-xs bg-green-100 text-green-600 px-2 py-1 rounded-full">
                    Personal Growth 🌱
                  </span>
                  <span className="text-xs bg-blue-100 text-blue-600 px-2 py-1 rounded-full">
                    Helped Others 💫
                  </span>
                </div>
              </motion.div>
            </div>

            {/* Center node */}
            <motion.div
              whileHover={{ scale: 1.2, rotate: 360 }}
              className="w-8 h-8 rounded-full bg-gradient-to-br from-green-400 to-blue-400 shadow-lg z-10"
            />
          </motion.div>
        ))}
      </div>

      {/* Reflection Modal */}
      <AnimatePresence>
        {isReflecting && (
          <motion.div
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            className="fixed inset-0 bg-black/50 flex items-center justify-center z-50"
          >
            <motion.div
              initial={{ scale: 0.9, opacity: 0 }}
              animate={{ scale: 1, opacity: 1 }}
              exit={{ scale: 0.9, opacity: 0 }}
              className="bg-white p-6 rounded-xl max-w-md w-full mx-4"
            >
              <h3 className="text-xl font-bold mb-4">Add a Reflection ✨</h3>
              
              <textarea
                placeholder="What did you learn? How did it change your perspective?"
                className="w-full h-32 p-3 border rounded-lg mb-4"
              />

              <div className="flex justify-between">
                <div className="space-x-2">
                  <button
                    onClick={() => setIsReflecting(false)}
                    className="px-4 py-2 text-gray-600"
                  >
                    Cancel
                  </button>
                </div>
                <motion.button
                  whileHover={{ scale: 1.05 }}
                  whileTap={{ scale: 0.95 }}
                  onClick={() => {
                    // Handle save reflection
                    setIsReflecting(false);
                  }}
                  className="px-4 py-2 bg-gradient-to-r from-green-500 to-blue-500 text-white rounded-lg"
                >
                  Save Reflection 🌱
                </motion.button>
              </div>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
