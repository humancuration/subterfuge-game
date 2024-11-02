import { motion, AnimatePresence } from "framer-motion";
import { useState, useEffect } from "react";
import { useFetcher } from "@remix-run/react";
import * as d3 from "d3";

interface ActivityNode {
  id: string;
  type: "creation" | "collaboration" | "comment" | "share" | "like" | "fork";
  timestamp: string;
  user: {
    id: string;
    username: string;
    avatar: string;
  };
  content: {
    title: string;
    preview?: string;
    link?: string;
  };
  impact: number;
}

const nodeEmojis = {
  creation: "✨",
  collaboration: "🤝",
  comment: "💭",
  share: "📢",
  like: "💖",
  fork: "🔄"
};

const funMessages = {
  creation: ["New heat dropped! 🔥", "Look what they made! ✨", "Creative genius moment! 🎨"],
  collaboration: ["Dream team alert! 👯‍♂️", "Power combo! ⚡", "Squad goals! 🎯"],
  comment: ["Spitting facts! 📝", "The discourse! 🗣️", "Big brain energy! 🧠"],
  share: ["Spread the word! 📣", "Going viral! 🚀", "Breaking the internet! 🌐"],
  like: ["Feeling the love! 💕", "Main character energy! 👑", "We stan! 🌟"],
  fork: ["Remix time! 🎵", "Evolution moment! 🧬", "Building on greatness! 🏗️"]
};

export function ActivityFlow() {
  const [activities, setActivities] = useState<ActivityNode[]>([]);
  const [selectedNode, setSelectedNode] = useState<ActivityNode | null>(null);
  const [flowDirection, setFlowDirection] = useState<"horizontal" | "vertical">("horizontal");
  const fetcher = useFetcher();

  useEffect(() => {
    // Fetch initial data
    fetcher.load("/api/activity-flow");
  }, []);

  useEffect(() => {
    if (fetcher.data) {
      setActivities(fetcher.data.activities);
    }
  }, [fetcher.data]);

  return (
    <div className="activity-flow p-6 bg-white rounded-xl shadow-lg">
      <div className="flex justify-between items-center mb-8">
        <h2 className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-purple-600 to-blue-600">
          Community Flow ✨
        </h2>
        <div className="flex gap-2">
          <motion.button
            whileHover={{ scale: 1.05 }}
            whileTap={{ scale: 0.95 }}
            onClick={() => setFlowDirection(flowDirection === "horizontal" ? "vertical" : "horizontal")}
            className="px-4 py-2 bg-gray-100 rounded-full text-sm font-medium"
          >
            {flowDirection === "horizontal" ? "↔️" : "↕️"} Flow
          </motion.button>
        </div>
      </div>

      <motion.div
        layout
        className={`flow-container ${flowDirection === "horizontal" ? "flex" : "space-y-4"}`}
      >
        <AnimatePresence mode="popLayout">
          {activities.map((activity, index) => (
            <motion.div
              key={activity.id}
              initial={{ opacity: 0, scale: 0.8 }}
              animate={{ opacity: 1, scale: 1 }}
              exit={{ opacity: 0, scale: 0.8 }}
              whileHover={{ scale: 1.05 }}
              onClick={() => setSelectedNode(activity)}
              className={`
                relative p-4 bg-gradient-to-br from-purple-50 to-blue-50 rounded-lg shadow-md
                ${flowDirection === "horizontal" ? "flex-shrink-0 w-72 mx-2" : ""}
              `}
            >
              {/* Connection lines */}
              {index < activities.length - 1 && (
                <motion.div
                  className={`connection-line ${
                    flowDirection === "horizontal" 
                      ? "absolute right-0 top-1/2 w-4 h-0.5" 
                      : "absolute bottom-0 left-1/2 w-0.5 h-4"
                  } bg-gradient-to-r from-purple-300 to-blue-300`}
                />
              )}

              {/* Activity content */}
              <div className="flex items-start gap-3">
                <div className="text-2xl">{nodeEmojis[activity.type]}</div>
                <div>
                  <div className="flex items-center gap-2">
                    <img
                      src={activity.user.avatar}
                      alt={activity.user.username}
                      className="w-6 h-6 rounded-full"
                    />
                    <span className="text-sm font-medium">{activity.user.username}</span>
                  </div>
                  <h3 className="font-medium mt-1">{activity.content.title}</h3>
                  {activity.content.preview && (
                    <p className="text-sm text-gray-600 mt-1">{activity.content.preview}</p>
                  )}
                  <div className="flex items-center gap-2 mt-2">
                    <span className="text-xs bg-purple-100 text-purple-600 px-2 py-1 rounded-full">
                      {activity.type}
                    </span>
                    <span className="text-xs text-gray-500">
                      {new Date(activity.timestamp).toLocaleTimeString()}
                    </span>
                  </div>
                </div>
              </div>

              {/* Impact indicator */}
              <motion.div
                className="absolute -top-2 -right-2 w-6 h-6 bg-gradient-to-br from-yellow-400 to-orange-400 rounded-full flex items-center justify-center text-white text-xs font-bold shadow-lg"
                animate={{
                  scale: [1, 1.2, 1],
                  rotate: [0, 10, -10, 0],
                }}
                transition={{
                  duration: 2,
                  repeat: Infinity,
                  ease: "easeInOut"
                }}
              >
                {activity.impact}
              </motion.div>
            </motion.div>
          ))}
        </AnimatePresence>
      </motion.div>

      {/* Selected node details */}
      <AnimatePresence>
        {selectedNode && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: 20 }}
            className="fixed inset-0 bg-black/50 flex items-center justify-center p-4 z-50"
            onClick={() => setSelectedNode(null)}
          >
            <motion.div
              className="bg-white rounded-xl p-6 max-w-md w-full"
              onClick={e => e.stopPropagation()}
            >
              <div className="flex items-center gap-3 mb-4">
                <span className="text-3xl">{nodeEmojis[selectedNode.type]}</span>
                <div>
                  <h3 className="text-xl font-bold">{selectedNode.content.title}</h3>
                  <p className="text-sm text-gray-500">
                    by {selectedNode.user.username}
                  </p>
                </div>
              </div>

              <div className="space-y-4">
                {selectedNode.content.preview && (
                  <p className="text-gray-600">{selectedNode.content.preview}</p>
                )}
                
                <div className="flex flex-wrap gap-2">
                  <span className="px-3 py-1 bg-purple-100 text-purple-600 rounded-full text-sm">
                    Impact: {selectedNode.impact}
                  </span>
                  <span className="px-3 py-1 bg-blue-100 text-blue-600 rounded-full text-sm">
                    {new Date(selectedNode.timestamp).toLocaleString()}
                  </span>
                </div>

                {selectedNode.content.link && (
                  <motion.a
                    href={selectedNode.content.link}
                    whileHover={{ scale: 1.05 }}
                    whileTap={{ scale: 0.95 }}
                    className="block w-full py-2 text-center bg-gradient-to-r from-purple-500 to-blue-500 text-white rounded-lg font-medium mt-4"
                  >
                    Check it out! ✨
                  </motion.a>
                )}
              </div>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
