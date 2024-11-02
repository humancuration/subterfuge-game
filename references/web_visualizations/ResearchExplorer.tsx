import { motion, AnimatePresence } from "framer-motion";
import { useState } from "react";
import { useFetcher } from "@remix-run/react";
import * as d3 from "d3";
import confetti from "canvas-confetti";

interface Paper {
  id: string;
  title: string;
  abstract: string;
  authors: Author[];
  keywords: string[];
  field: string;
  publicationDate: string;
  citations: number;
  openAccess: boolean;
  peerReviews: PeerReview[];
  relatedPapers: string[];
  communityEngagement: {
    discussions: number;
    annotations: number;
    implementations: number;
  };
  metrics: {
    readability: number;
    reproducibility: number;
    impact: number;
  };
}

interface Author {
  id: string;
  name: string;
  avatar: string;
  institution: string;
  contributions: string[];
}

interface PeerReview {
  id: string;
  reviewer: {
    id: string;
    name: string;
    expertise: string[];
    reputation: number;
  };
  content: string;
  rating: number;
  timestamp: string;
  verified: boolean;
}

const fieldEmojis: Record<string, string> = {
  "computer-science": "💻",
  "biology": "🧬",
  "physics": "⚛️",
  "chemistry": "🧪",
  "mathematics": "🔢",
  "medicine": "🏥",
  "psychology": "🧠",
  "environmental": "🌍",
  "ai": "🤖"
};

const funMessages = {
  highImpact: [
    "This paper is serving! 💅",
    "Main character of academia! 👑",
    "The math is mathing! ✨",
    "Science but make it fun! 🌟"
  ],
  openAccess: [
    "Knowledge wants to be free! 🦋",
    "Sharing is caring! 💝",
    "Science for everyone! 🌈",
    "No paywalls here bestie! 🚫"
  ]
};

export function ResearchExplorer() {
  const [selectedPaper, setSelectedPaper] = useState<Paper | null>(null);
  const [viewMode, setViewMode] = useState<"graph" | "list" | "map">("graph");
  const [showReviewModal, setShowReviewModal] = useState(false);
  const fetcher = useFetcher();

  const handleContribute = (type: "review" | "discussion" | "implementation") => {
    confetti({
      particleCount: 50,
      spread: 60,
      colors: ['#4CAF50', '#2196F3', '#9C27B0']
    });
  };

  return (
    <div className="research-explorer p-6 bg-white rounded-xl shadow-lg">
      <div className="flex justify-between items-center mb-8">
        <div>
          <h2 className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-purple-600 to-blue-600">
            Research Explorer 🔬
          </h2>
          <p className="text-gray-600 mt-1">
            Discover, review, and build upon research together!
          </p>
        </div>
        <div className="flex gap-2">
          {["graph", "list", "map"].map((mode) => (
            <motion.button
              key={mode}
              whileHover={{ scale: 1.05 }}
              whileTap={{ scale: 0.95 }}
              onClick={() => setViewMode(mode as typeof viewMode)}
              className={`px-4 py-2 rounded-full ${
                viewMode === mode
                  ? "bg-gradient-to-r from-purple-500 to-blue-500 text-white"
                  : "bg-gray-100 text-gray-700 hover:bg-gray-200"
              }`}
            >
              {mode.charAt(0).toUpperCase() + mode.slice(1)}
            </motion.button>
          ))}
        </div>
      </div>

      {/* Paper Cards */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {[1, 2, 3].map((_, index) => (
          <motion.div
            key={index}
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: index * 0.1 }}
            className="p-6 bg-gradient-to-br from-purple-50 to-blue-50 rounded-xl shadow-md"
          >
            <div className="flex justify-between items-start mb-4">
              <div>
                <span className="text-2xl">{fieldEmojis["computer-science"]}</span>
                <h3 className="font-bold mt-2">Awesome Research Paper {index + 1}</h3>
                <div className="flex items-center gap-2 mt-1">
                  <span className="text-sm text-gray-600">by Research Team {index + 1}</span>
                  {index % 2 === 0 && (
                    <span className="px-2 py-1 bg-green-100 text-green-600 rounded-full text-xs">
                      Open Access ✨
                    </span>
                  )}
                </div>
              </div>
            </div>

            <p className="text-gray-700 mb-4">
              A groundbreaking study on making science more accessible and fun!
            </p>

            {/* Metrics */}
            <div className="grid grid-cols-3 gap-2 mb-4">
              <div className="text-center p-2 bg-white/50 rounded-lg">
                <div className="text-lg font-bold">42</div>
                <div className="text-xs text-gray-600">Citations</div>
              </div>
              <div className="text-center p-2 bg-white/50 rounded-lg">
                <div className="text-lg font-bold">89%</div>
                <div className="text-xs text-gray-600">Reproducible</div>
              </div>
              <div className="text-center p-2 bg-white/50 rounded-lg">
                <div className="text-lg font-bold">12</div>
                <div className="text-xs text-gray-600">Reviews</div>
              </div>
            </div>

            {/* Action Buttons */}
            <div className="flex justify-between items-center">
              <div className="flex gap-2">
                <motion.button
                  whileHover={{ scale: 1.05 }}
                  whileTap={{ scale: 0.95 }}
                  onClick={() => handleContribute("review")}
                  className="p-2 bg-blue-100 text-blue-600 rounded-full"
                >
                  📝 Review
                </motion.button>
                <motion.button
                  whileHover={{ scale: 1.05 }}
                  whileTap={{ scale: 0.95 }}
                  onClick={() => handleContribute("discussion")}
                  className="p-2 bg-purple-100 text-purple-600 rounded-full"
                >
                  💭 Discuss
                </motion.button>
              </div>
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                onClick={() => setSelectedPaper({ id: `paper-${index}` } as Paper)}
                className="px-4 py-2 bg-gradient-to-r from-purple-500 to-blue-500 text-white rounded-full text-sm"
              >
                Read More ✨
              </motion.button>
            </div>
          </motion.div>
        ))}
      </div>

      {/* Paper Detail Modal */}
      <AnimatePresence>
        {selectedPaper && (
          <motion.div
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            className="fixed inset-0 bg-black/50 flex items-center justify-center z-50 p-4"
          >
            <motion.div
              initial={{ scale: 0.9, opacity: 0 }}
              animate={{ scale: 1, opacity: 1 }}
              exit={{ scale: 0.9, opacity: 0 }}
              className="bg-white rounded-xl max-w-4xl w-full max-h-[90vh] overflow-y-auto"
            >
              {/* Modal content would go here */}
              <div className="p-6">
                <button
                  onClick={() => setSelectedPaper(null)}
                  className="absolute top-4 right-4 text-gray-500 hover:text-gray-700"
                >
                  ✕
                </button>
                
                {/* Paper content, reviews, discussions, etc. */}
              </div>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
