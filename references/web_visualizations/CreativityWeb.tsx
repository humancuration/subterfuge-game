import { useEffect, useRef, useState } from "react";
import { motion, AnimatePresence } from "framer-motion";
import * as d3 from "d3";
import { useFetcher } from "@remix-run/react";

interface CreativeNode {
  id: string;
  type: "project" | "idea" | "resource" | "person" | "ai";
  title: string;
  description: string;
  tags: string[];
  connections: string[];
  energy: number; // How active/influential the node is
  creator: {
    id: string;
    username: string;
    avatar: string;
  };
}

interface CreativityWebProps {
  nodes: CreativeNode[];
  onNodeClick?: (node: CreativeNode) => void;
  highlightTags?: string[];
}

const nodeColors = {
  project: "#8B5CF6", // Purple
  idea: "#EC4899",    // Pink
  resource: "#10B981", // Green
  person: "#3B82F6",  // Blue
  ai: "#F59E0B"       // Orange
};

const nodeEmojis = {
  project: "🚀",
  idea: "💡",
  resource: "📚",
  person: "👤",
  ai: "🤖"
};

export function CreativityWeb({ nodes, onNodeClick, highlightTags }: CreativityWebProps) {
  const svgRef = useRef<SVGSVGElement>(null);
  const [selectedNode, setSelectedNode] = useState<CreativeNode | null>(null);
  const [hoveredNode, setHoveredNode] = useState<CreativeNode | null>(null);
  const fetcher = useFetcher();

  useEffect(() => {
    if (!svgRef.current) return;

    const width = 800;
    const height = 600;

    // Clear previous visualization
    d3.select(svgRef.current).selectAll("*").remove();

    const svg = d3.select(svgRef.current)
      .attr("viewBox", `0 0 ${width} ${height}`)
      .attr("style", "max-width: 100%; height: auto;");

    // Create force simulation
    const simulation = d3.forceSimulation(nodes)
      .force("charge", d3.forceManyBody().strength(-200))
      .force("center", d3.forceCenter(width / 2, height / 2))
      .force("collision", d3.forceCollide().radius(50))
      .force("link", d3.forceLink(nodes.flatMap(node => 
        node.connections.map(targetId => ({
          source: node.id,
          target: targetId
        }))
      )).distance(100));

    // Create gradient definitions
    const defs = svg.append("defs");
    Object.entries(nodeColors).forEach(([type, color]) => {
      const gradient = defs.append("radialGradient")
        .attr("id", `gradient-${type}`)
        .attr("cx", "50%")
        .attr("cy", "50%")
        .attr("r", "50%");

      gradient.append("stop")
        .attr("offset", "0%")
        .attr("style", `stop-color: ${color}; stop-opacity: 0.2`);

      gradient.append("stop")
        .attr("offset", "100%")
        .attr("style", `stop-color: ${color}; stop-opacity: 0.8`);
    });

    // Create links with animated particles
    const links = svg.append("g")
      .selectAll("path")
      .data(nodes.flatMap(node => 
        node.connections.map(targetId => ({
          source: node.id,
          target: targetId
        }))
      ))
      .join("path")
      .attr("class", "connection")
      .attr("stroke", "#ddd")
      .attr("stroke-width", 2)
      .attr("fill", "none");

    // Create nodes
    const nodeGroups = svg.append("g")
      .selectAll("g")
      .data(nodes)
      .join("g")
      .attr("class", "node")
      .call(d3.drag<SVGGElement, CreativeNode>()
        .on("start", dragStarted)
        .on("drag", dragged)
        .on("end", dragEnded));

    // Add node circles with gradients
    nodeGroups.append("circle")
      .attr("r", d => 20 + (d.energy * 5))
      .attr("fill", d => `url(#gradient-${d.type})`)
      .attr("stroke", d => nodeColors[d.type])
      .attr("stroke-width", 2);

    // Add emoji icons
    nodeGroups.append("text")
      .attr("text-anchor", "middle")
      .attr("dy", "0.3em")
      .attr("font-size", "20px")
      .text(d => nodeEmojis[d.type]);

    // Add hover effects
    nodeGroups
      .on("mouseover", (event, d) => {
        setHoveredNode(d);
        d3.select(event.currentTarget)
          .transition()
          .duration(200)
          .attr("transform", "scale(1.1)");
      })
      .on("mouseout", (event) => {
        setHoveredNode(null);
        d3.select(event.currentTarget)
          .transition()
          .duration(200)
          .attr("transform", "scale(1)");
      })
      .on("click", (_, d) => {
        setSelectedNode(d);
        if (onNodeClick) onNodeClick(d);
      });

    // Update positions on simulation tick
    simulation.on("tick", () => {
      links.attr("d", (d: any) => {
        const dx = d.target.x - d.source.x;
        const dy = d.target.y - d.source.y;
        const dr = Math.sqrt(dx * dx + dy * dy);
        return `M${d.source.x},${d.source.y}A${dr},${dr} 0 0,1 ${d.target.x},${d.target.y}`;
      });

      nodeGroups.attr("transform", (d: any) => `translate(${d.x},${d.y})`);
    });

    return () => {
      simulation.stop();
    };
  }, [nodes, highlightTags]);

  return (
    <div className="creativity-web relative">
      <svg
        ref={svgRef}
        className="w-full h-[600px] bg-gradient-to-br from-gray-50 to-white rounded-xl shadow-inner"
      />

      {/* Hover tooltip */}
      <AnimatePresence>
        {hoveredNode && (
          <motion.div
            initial={{ opacity: 0, y: 10 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: 10 }}
            className="absolute pointer-events-none bg-white p-4 rounded-lg shadow-lg"
            style={{
              left: `${(hoveredNode as any).x + 30}px`,
              top: `${(hoveredNode as any).y - 20}px`
            }}
          >
            <h3 className="font-bold text-gray-800">{hoveredNode.title}</h3>
            <p className="text-sm text-gray-600">{hoveredNode.description}</p>
            <div className="flex flex-wrap gap-1 mt-2">
              {hoveredNode.tags.map(tag => (
                <span
                  key={tag}
                  className="px-2 py-1 bg-gray-100 text-gray-600 rounded-full text-xs"
                >
                  #{tag}
                </span>
              ))}
            </div>
          </motion.div>
        )}
      </AnimatePresence>

      {/* Selected node details */}
      <AnimatePresence>
        {selectedNode && (
          <motion.div
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.9 }}
            className="absolute top-4 right-4 bg-white p-6 rounded-xl shadow-lg max-w-md"
          >
            <div className="flex justify-between items-start mb-4">
              <div className="flex items-center gap-3">
                <div 
                  className="w-12 h-12 rounded-full flex items-center justify-center text-2xl"
                  style={{ backgroundColor: `${nodeColors[selectedNode.type]}20` }}
                >
                  {nodeEmojis[selectedNode.type]}
                </div>
                <div>
                  <h3 className="font-bold text-gray-800">{selectedNode.title}</h3>
                  <p className="text-sm text-gray-500">
                    by {selectedNode.creator.username}
                  </p>
                </div>
              </div>
              <button
                onClick={() => setSelectedNode(null)}
                className="text-gray-400 hover:text-gray-600"
              >
                ✕
              </button>
            </div>

            <p className="text-gray-600 mb-4">{selectedNode.description}</p>

            <div className="flex flex-wrap gap-2 mb-4">
              {selectedNode.tags.map(tag => (
                <span
                  key={tag}
                  className="px-2 py-1 bg-gray-100 text-gray-600 rounded-full text-sm"
                >
                  #{tag}
                </span>
              ))}
            </div>

            <div className="flex justify-end gap-2">
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                className="px-4 py-2 bg-gradient-to-r from-purple-500 to-blue-500 text-white rounded-lg"
                onClick={() => {
                  fetcher.submit(
                    { nodeId: selectedNode.id, intent: "connect" },
                    { method: "post" }
                  );
                }}
              >
                Connect & Collaborate ✨
              </motion.button>
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
