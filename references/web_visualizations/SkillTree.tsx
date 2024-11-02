import { motion, AnimatePresence } from "framer-motion";
import { useState, useRef, useEffect } from "react";
import { useFetcher } from "@remix-run/react";
import * as d3 from "d3";
import confetti from "canvas-confetti";

interface Skill {
  id: string;
  name: string;
  description: string;
  icon: string;
  level: number;
  maxLevel: number;
  xp: number;
  xpRequired: number;
  unlocked: boolean;
  category: "tech" | "creative" | "business" | "science" | "community";
  prerequisites: string[];
  bonuses: {
    type: "avatar" | "pet" | "collab" | "impact";
    description: string;
  }[];
}

interface SkillNode extends d3.SimulationNodeDatum {
  id: string;
  skill: Skill;
  x?: number;
  y?: number;
}

interface SkillLink extends d3.SimulationLinkDatum<SkillNode> {
  source: string;
  target: string;
}

const categoryColors = {
  tech: "from-blue-400 to-cyan-400",
  creative: "from-purple-400 to-pink-400",
  business: "from-green-400 to-emerald-400",
  science: "from-yellow-400 to-orange-400",
  community: "from-red-400 to-rose-400"
};

const funMessages = {
  levelUp: [
    "Level up bestie! ✨",
    "Skills on point! 💅",
    "You're literally glowing! 🌟",
    "Main character energy! 👑",
    "We love growth! 🌱",
    "Slay! 💖"
  ],
  unlock: [
    "New skill unlocked! 🔓",
    "The power is yours! ⚡",
    "Character development! 📈",
    "You're evolving! 🦋",
    "Power level rising! 💪"
  ]
};

export function SkillTree() {
  const svgRef = useRef<SVGSVGElement>(null);
  const [selectedSkill, setSelectedSkill] = useState<Skill | null>(null);
  const [showLevelUp, setShowLevelUp] = useState(false);
  const fetcher = useFetcher();

  useEffect(() => {
    if (!svgRef.current) return;

    const width = 800;
    const height = 600;
    const svg = d3.select(svgRef.current);

    // Clear previous visualization
    svg.selectAll("*").remove();

    // Create gradient definitions for paths
    const defs = svg.append("defs");
    Object.entries(categoryColors).forEach(([category, gradient]) => {
      const linearGradient = defs.append("linearGradient")
        .attr("id", `path-gradient-${category}`)
        .attr("gradientUnits", "userSpaceOnUse")
        .attr("x1", "0%")
        .attr("y1", "0%")
        .attr("x2", "100%")
        .attr("y2", "0%");

      linearGradient.append("stop")
        .attr("offset", "0%")
        .attr("class", `${gradient.split(" ")[0]}`);

      linearGradient.append("stop")
        .attr("offset", "100%")
        .attr("class", `${gradient.split(" ")[1]}`);
    });

    // Create force simulation
    const simulation = d3.forceSimulation<SkillNode>()
      .force("link", d3.forceLink<SkillNode, SkillLink>().id(d => d.id))
      .force("charge", d3.forceManyBody().strength(-200))
      .force("center", d3.forceCenter(width / 2, height / 2))
      .force("collision", d3.forceCollide().radius(50));

    // Draw skill connections
    const links = svg.append("g")
      .selectAll("path")
      .data(skillLinks)
      .join("path")
      .attr("class", "skill-link")
      .attr("stroke-width", 3)
      .attr("stroke-linecap", "round");

    // Create skill nodes
    const nodes = svg.append("g")
      .selectAll("g")
      .data(skillNodes)
      .join("g")
      .attr("class", "skill-node")
      .call(d3.drag<SVGGElement, SkillNode>()
        .on("start", dragStarted)
        .on("drag", dragged)
        .on("end", dragEnded));

    // Add node visuals
    nodes.append("circle")
      .attr("r", 25)
      .attr("class", d => `bg-gradient-to-br ${categoryColors[d.skill.category]}`);

    nodes.append("text")
      .attr("dy", "0.35em")
      .attr("text-anchor", "middle")
      .text(d => d.skill.icon)
      .style("font-size", "20px");

    // Add XP progress rings
    nodes.append("circle")
      .attr("r", 28)
      .attr("class", "progress-ring")
      .attr("fill", "none")
      .attr("stroke-width", 3)
      .attr("stroke-dasharray", (d) => {
        const circumference = 2 * Math.PI * 28;
        const progress = d.skill.xp / d.skill.xpRequired;
        return `${circumference * progress} ${circumference}`;
      });

    // Update positions on simulation tick
    simulation.on("tick", () => {
      links
        .attr("d", (d: any) => {
          const dx = d.target.x - d.source.x;
          const dy = d.target.y - d.source.y;
          const dr = Math.sqrt(dx * dx + dy * dy);
          return `M${d.source.x},${d.source.y}A${dr},${dr} 0 0,1 ${d.target.x},${d.target.y}`;
        });

      nodes
        .attr("transform", (d: any) => `translate(${d.x},${d.y})`);
    });

    // Handle node clicks
    nodes.on("click", (event, d) => {
      setSelectedSkill(d.skill);
    });

  }, []);

  const handleLevelUp = (skill: Skill) => {
    fetcher.submit(
      { skillId: skill.id, intent: "levelUp" },
      { method: "post", action: "/api/skills" }
    );

    confetti({
      particleCount: 100,
      spread: 70,
      origin: { y: 0.6 }
    });

    setShowLevelUp(true);
    setTimeout(() => setShowLevelUp(false), 2000);
  };

  return (
    <div className="skill-tree relative">
      <svg
        ref={svgRef}
        className="w-full h-[600px] bg-gradient-to-br from-gray-50 to-white rounded-xl"
        viewBox="0 0 800 600"
      />

      {/* Skill Details Panel */}
      <AnimatePresence>
        {selectedSkill && (
          <motion.div
            initial={{ opacity: 0, x: 20 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: 20 }}
            className="absolute top-4 right-4 w-80 bg-white rounded-xl shadow-lg p-6"
          >
            <div className="flex items-center gap-4 mb-4">
              <div className={`w-12 h-12 rounded-full flex items-center justify-center text-2xl bg-gradient-to-br ${categoryColors[selectedSkill.category]}`}>
                {selectedSkill.icon}
              </div>
              <div>
                <h3 className="font-bold text-lg">{selectedSkill.name}</h3>
                <p className="text-sm text-gray-500">Level {selectedSkill.level}/{selectedSkill.maxLevel}</p>
              </div>
            </div>

            <p className="text-gray-600 mb-4">{selectedSkill.description}</p>

            {/* XP Progress */}
            <div className="mb-4">
              <div className="flex justify-between text-sm text-gray-600 mb-1">
                <span>XP Progress</span>
                <span>{selectedSkill.xp}/{selectedSkill.xpRequired}</span>
              </div>
              <div className="h-2 bg-gray-100 rounded-full overflow-hidden">
                <motion.div
                  className={`h-full bg-gradient-to-r ${categoryColors[selectedSkill.category]}`}
                  initial={{ width: 0 }}
                  animate={{ width: `${(selectedSkill.xp / selectedSkill.xpRequired) * 100}%` }}
                />
              </div>
            </div>

            {/* Bonuses */}
            <div className="space-y-2 mb-4">
              <h4 className="font-medium text-sm">Bonuses</h4>
              {selectedSkill.bonuses.map((bonus, index) => (
                <div
                  key={index}
                  className="flex items-center gap-2 text-sm text-gray-600"
                >
                  <span className="text-lg">
                    {bonus.type === "avatar" ? "👤" :
                     bonus.type === "pet" ? "🐾" :
                     bonus.type === "collab" ? "🤝" : "✨"}
                  </span>
                  {bonus.description}
                </div>
              ))}
            </div>

            {selectedSkill.level < selectedSkill.maxLevel && (
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                onClick={() => handleLevelUp(selectedSkill)}
                className={`w-full py-2 text-white rounded-lg bg-gradient-to-r ${categoryColors[selectedSkill.category]}`}
              >
                Level Up! ✨
              </motion.button>
            )}
          </motion.div>
        )}
      </AnimatePresence>

      {/* Level Up Animation */}
      <AnimatePresence>
        {showLevelUp && (
          <motion.div
            initial={{ opacity: 0, scale: 0.5, y: 20 }}
            animate={{ opacity: 1, scale: 1, y: 0 }}
            exit={{ opacity: 0, scale: 0.5, y: -20 }}
            className="fixed inset-0 flex items-center justify-center pointer-events-none"
          >
            <div className="text-4xl font-bold text-white text-center">
              {funMessages.levelUp[Math.floor(Math.random() * funMessages.levelUp.length)]}
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}

// Helper functions for D3 drag behavior
function dragStarted(event: any) {
  if (!event.active) event.simulation.alphaTarget(0.3).restart();
  event.subject.fx = event.subject.x;
  event.subject.fy = event.subject.y;
}

function dragged(event: any) {
  event.subject.fx = event.x;
  event.subject.fy = event.y;
}

function dragEnded(event: any) {
  if (!event.active) event.simulation.alphaTarget(0);
  event.subject.fx = null;
  event.subject.fy = null;
}
