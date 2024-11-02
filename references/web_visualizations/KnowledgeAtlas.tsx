import { useEffect, useRef, useState } from "react";
import { Canvas, useFrame, useThree } from "@react-three/fiber";
import { OrbitControls, Text, Html } from "@react-three/drei";
import { motion, AnimatePresence } from "framer-motion";
import { useFetcher } from "@remix-run/react";
import * as THREE from "three";

interface KnowledgeNode {
  id: string;
  title: string;
  type: "known" | "frontier" | "unexplored";
  category: string;
  connections: string[];
  position: [number, number, number];
  contributors: {
    id: string;
    username: string;
    contribution: string;
  }[];
  explorationProgress?: number;
  potentialDirections?: string[];
}

interface KnowledgeCluster {
  id: string;
  nodes: KnowledgeNode[];
  center: [number, number, number];
  radius: number;
  density: number;
}

const nodeColors = {
  known: "#4CAF50",      // Established knowledge
  frontier: "#FF9800",   // Active research/development
  unexplored: "#9C27B0"  // Potential discoveries
};

const particleColors = {
  connection: "#2196F3",
  discovery: "#F44336",
  collaboration: "#4CAF50"
};

function Node({ node, onSelect }: { node: KnowledgeNode; onSelect: (node: KnowledgeNode) => void }) {
  const mesh = useRef<THREE.Mesh>();
  const [hovered, setHovered] = useState(false);
  
  useFrame((state) => {
    if (mesh.current) {
      // Gentle floating animation
      mesh.current.position.y += Math.sin(state.clock.getElapsedTime() * 2) * 0.001;
      
      if (node.type === "frontier") {
        // Pulse effect for frontier nodes
        mesh.current.scale.setScalar(
          1 + Math.sin(state.clock.getElapsedTime() * 3) * 0.1
        );
      }
    }
  });

  return (
    <mesh
      ref={mesh}
      position={new THREE.Vector3(...node.position)}
      onClick={() => onSelect(node)}
      onPointerOver={() => setHovered(true)}
      onPointerOut={() => setHovered(false)}
    >
      <sphereGeometry args={[hovered ? 1.2 : 1, 32, 32]} />
      <meshPhongMaterial
        color={nodeColors[node.type]}
        transparent
        opacity={node.type === "unexplored" ? 0.5 : 0.8}
      />
      {hovered && (
        <Html>
          <div className="bg-white/90 p-2 rounded shadow-lg">
            <h3 className="font-bold">{node.title}</h3>
            <p className="text-sm text-gray-600">{node.category}</p>
          </div>
        </Html>
      )}
    </mesh>
  );
}

function ConnectionLines({ nodes }: { nodes: KnowledgeNode[] }) {
  const lines = useRef<THREE.LineSegments>();

  useEffect(() => {
    if (!lines.current) return;

    const positions: number[] = [];
    const colors: number[] = [];

    nodes.forEach(node => {
      node.connections.forEach(targetId => {
        const target = nodes.find(n => n.id === targetId);
        if (target) {
          positions.push(...node.position, ...target.position);
          const color = new THREE.Color(particleColors.connection);
          colors.push(color.r, color.g, color.b);
          colors.push(color.r, color.g, color.b);
        }
      });
    });

    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.Float32BufferAttribute(positions, 3));
    geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));
    
    lines.current.geometry = geometry;
  }, [nodes]);

  return (
    <lineSegments ref={lines}>
      <lineBasicMaterial vertexColors transparent opacity={0.3} />
    </lineSegments>
  );
}

export function KnowledgeAtlas() {
  const [selectedNode, setSelectedNode] = useState<KnowledgeNode | null>(null);
  const [viewMode, setViewMode] = useState<"explore" | "contribute" | "connect">("explore");
  const fetcher = useFetcher();

  const handleNodeSelect = (node: KnowledgeNode) => {
    setSelectedNode(node);
  };

  const handleContribute = (contribution: string) => {
    fetcher.submit(
      { 
        nodeId: selectedNode?.id,
        contribution,
        intent: "contribute" 
      },
      { method: "post" }
    );
  };

  return (
    <div className="knowledge-atlas h-screen relative">
      {/* 3D Visualization */}
      <Canvas camera={{ position: [0, 0, 50] }}>
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        <OrbitControls 
          enableDamping
          dampingFactor={0.05}
          rotateSpeed={0.5}
          zoomSpeed={0.5}
        />
        
        {/* Render nodes */}
        {mockNodes.map(node => (
          <Node key={node.id} node={node} onSelect={handleNodeSelect} />
        ))}

        {/* Render connections */}
        <ConnectionLines nodes={mockNodes} />

        {/* Particle systems for various effects */}
        <Points>
          {/* Discovery particles */}
          {/* Collaboration streams */}
          {/* Energy flows */}
        </Points>
      </Canvas>

      {/* UI Overlay */}
      <div className="absolute top-4 left-4 space-y-4">
        <div className="bg-white/90 p-4 rounded-lg shadow-lg">
          <h2 className="text-xl font-bold mb-2">Atlas of Knowledge</h2>
          <div className="flex gap-2">
            {["explore", "contribute", "connect"].map(mode => (
              <motion.button
                key={mode}
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                onClick={() => setViewMode(mode as typeof viewMode)}
                className={`px-3 py-1 rounded-full ${
                  viewMode === mode
                    ? "bg-blue-500 text-white"
                    : "bg-gray-100"
                }`}
              >
                {mode.charAt(0).toUpperCase() + mode.slice(1)}
              </motion.button>
            ))}
          </div>
        </div>

        <div className="bg-white/90 p-4 rounded-lg shadow-lg">
          <h3 className="font-bold mb-2">Legend</h3>
          <div className="space-y-2 text-sm">
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded-full" style={{ backgroundColor: nodeColors.known }} />
              <span>Established Knowledge</span>
            </div>
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded-full" style={{ backgroundColor: nodeColors.frontier }} />
              <span>Active Research</span>
            </div>
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded-full" style={{ backgroundColor: nodeColors.unexplored }} />
              <span>Unexplored Territory</span>
            </div>
          </div>
        </div>
      </div>

      {/* Selected Node Details */}
      <AnimatePresence>
        {selectedNode && (
          <motion.div
            initial={{ opacity: 0, x: 100 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: 100 }}
            className="absolute top-4 right-4 w-96 bg-white/90 p-6 rounded-lg shadow-lg"
          >
            <h3 className="text-xl font-bold mb-2">{selectedNode.title}</h3>
            <p className="text-gray-600 mb-4">{selectedNode.category}</p>

            {selectedNode.type === "frontier" && (
              <div className="mb-4">
                <h4 className="font-medium mb-2">Exploration Progress</h4>
                <div className="h-2 bg-gray-200 rounded-full overflow-hidden">
                  <motion.div
                    className="h-full bg-blue-500"
                    initial={{ width: 0 }}
                    animate={{ width: `${selectedNode.explorationProgress}%` }}
                  />
                </div>
              </div>
            )}

            <div className="mb-4">
              <h4 className="font-medium mb-2">Contributors</h4>
              <div className="space-y-2">
                {selectedNode.contributors.map(contributor => (
                  <div key={contributor.id} className="text-sm">
                    <span className="font-medium">{contributor.username}</span>
                    <p className="text-gray-500">{contributor.contribution}</p>
                  </div>
                ))}
              </div>
            </div>

            {selectedNode.type === "unexplored" && (
              <div className="mb-4">
                <h4 className="font-medium mb-2">Potential Directions</h4>
                <div className="space-y-1">
                  {selectedNode.potentialDirections?.map(direction => (
                    <div key={direction} className="text-sm text-blue-600">
                      → {direction}
                    </div>
                  ))}
                </div>
              </div>
            )}

            <div className="flex justify-end">
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                onClick={() => {
                  // Handle contribution
                }}
                className="px-4 py-2 bg-blue-500 text-white rounded-lg"
              >
                Contribute to this Knowledge ✨
              </motion.button>
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}

// Mock data for demonstration
const mockNodes: KnowledgeNode[] = [
  // Example nodes would go here
];
