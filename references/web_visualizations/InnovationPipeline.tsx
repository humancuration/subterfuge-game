import { motion, AnimatePresence } from "framer-motion";
import { useState, useEffect } from "react";
import { useFetcher } from "@remix-run/react";

interface Innovation {
  id: string;
  title: string;
  description: string;
  stage: "ideation" | "research" | "prototype" | "testing" | "launch" | "scaling";
  creator: {
    id: string;
    username: string;
    avatar: string;
  };
  collaborators: number;
  momentum: number; // 0-100
  tags: string[];
  resources: string[];
}

const stageEmojis = {
  ideation: "💡",
  research: "🔬",
  prototype: "🛠️",
  testing: "🧪",
  launch: "🚀",
  scaling: "📈"
};

const stageColors = {
  ideation: "from-blue-400 to-purple-400",
  research: "from-purple-400 to-pink-400",
  prototype: "from-pink-400 to-red-400",
  testing: "from-red-400 to-orange-400",
  launch: "from-orange-400 to-yellow-400",
  scaling: "from-yellow-400 to-green-400"
};

export function InnovationPipeline() {
  const [activeStage, setActiveStage] = useState<keyof typeof stageEmojis>("ideation");
  const [flowParticles, setFlowParticles] = useState<Array<{ id: string; x: number }>>([]);
  const fetcher = useFetcher();

  // Simulate flowing particles
  useEffect(() => {
    const interval = setInterval(() => {
      setFlowParticles(current => {
        const newParticles = current
          .map(p => ({ ...p, x: p.x + 2 }))
          .filter(p => p.x < 100);

        if (Math.random() > 0.7) {
          newParticles.push({ id: Math.random().toString(), x: 0 });
        }

        return newParticles;
      });
    }, 50);

    return () => clearInterval(interval);
  }, []);

  return (
    <div className="innovation-pipeline p-6 bg-white rounded-xl shadow-lg">
      <div className="flex justify-between items-center mb-8">
        <h2 className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-purple-600 to-blue-600">
          Innovation Pipeline ✨
        </h2>
        <motion.button
          whileHover={{ scale: 1.05 }}
          whileTap={{ scale: 0.95 }}
          className="px-4 py-2 bg-gradient-to-r from-purple-500 to-blue-500 text-white rounded-lg"
          onClick={() => {
            // Handle new innovation
          }}
        >
          Add Innovation 🌟
        </motion.button>
      </div>

      {/* Pipeline Stages */}
      <div className="relative mb-8">
        <div className="absolute inset-0 bg-gradient-to-r from-purple-100 to-blue-100 rounded-lg" />
        <div className="relative flex justify-between p-4">
          {Object.entries(stageEmojis).map(([stage, emoji]) => (
            <motion.div
              key={stage}
              className={`relative ${
                activeStage === stage ? "scale-110" : "scale-100"
              }`}
            >
              <motion.div
                className={`w-12 h-12 rounded-full bg-gradient-to-br ${
                  stageColors[stage as keyof typeof stageColors]
                } flex items-center justify-center text-2xl shadow-lg`}
                whileHover={{ scale: 1.1, rotate: [0, -5, 5, 0] }}
              >
                {emoji}
              </motion.div>
              <div className="absolute -bottom-6 left-1/2 transform -translate-x-1/2 text-xs font-medium capitalize">
                {stage}
              </div>
            </motion.div>
          ))}
        </div>

        {/* Flow Particles */}
        <div className="absolute inset-0 overflow-hidden">
          {flowParticles.map(particle => (
            <motion.div
              key={particle.id}
              className="absolute top-1/2 transform -translate-y-1/2 w-2 h-2 rounded-full bg-gradient-to-r from-purple-500 to-blue-500"
              style={{ left: `${particle.x}%` }}
              initial={{ opacity: 0, scale: 0 }}
              animate={{ opacity: 1, scale: 1 }}
              exit={{ opacity: 0, scale: 0 }}
            />
          ))}
        </div>
      </div>

      {/* Innovation Cards */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {[1, 2, 3].map((_, index) => (
          <motion.div
            key={index}
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: index * 0.1 }}
            className="bg-gradient-to-br from-purple-50 to-blue-50 rounded-xl p-6 shadow-md"
          >
            <div className="flex justify-between items-start mb-4">
              <div className="flex items-center gap-3">
                <div className="w-10 h-10 rounded-full bg-gradient-to-br from-purple-400 to-blue-400" />
                <div>
                  <h3 className="font-bold">Cool Innovation {index + 1}</h3>
                  <p className="text-sm text-gray-600">by Innovator {index + 1}</p>
                </div>
              </div>
              <span className="text-2xl">{stageEmojis[activeStage]}</span>
            </div>

            <p className="text-gray-700 mb-4">
              An amazing innovation that's going to change the world! Using AI, blockchain, and good vibes ✨
            </p>

            {/* Momentum Bar */}
            <div className="mb-4">
              <div className="flex justify-between text-sm text-gray-600 mb-1">
                <span>Momentum</span>
                <span>78%</span>
              </div>
              <div className="h-2 bg-gray-100 rounded-full overflow-hidden">
                <motion.div
                  className="h-full bg-gradient-to-r from-purple-500 to-blue-500"
                  initial={{ width: 0 }}
                  animate={{ width: "78%" }}
                />
              </div>
            </div>

            <div className="flex flex-wrap gap-2 mb-4">
              {["ai", "blockchain", "sustainability"].map(tag => (
                <span
                  key={tag}
                  className="px-2 py-1 bg-white/50 rounded-full text-sm"
                >
                  #{tag}
                </span>
              ))}
            </div>

            <div className="flex justify-between items-center">
              <div className="text-sm text-gray-500">
                👥 {3 + index} Collaborators
              </div>
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                className="px-3 py-1 bg-gradient-to-r from-purple-500 to-blue-500 text-white rounded-full text-sm"
              >
                Join Project ✨
              </motion.button>
            </div>
          </motion.div>
        ))}
      </div>
    </div>
  );
}
