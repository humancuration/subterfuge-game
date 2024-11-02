import { useEffect, useRef } from "react";
import { motion, AnimatePresence } from "framer-motion";
import { Network, DataSet, Options } from 'vis-network';
import { useFetcher } from "@remix-run/react";

interface SupplyNode {
  id: string;
  type: "supplier" | "manufacturer" | "distributor" | "retailer";
  name: string;
  sustainabilityScore: number;
  verificationStatus: "verified" | "pending" | "unverified";
  location: {
    lat: number;
    lng: number;
    country: string;
  };
  impact: {
    environmental: number;
    social: number;
    economic: number;
  };
}

interface SupplyLink {
  id: string;
  source: string;
  target: string;
  type: "transport" | "processing";
  carbonEmission: number;
  distance: number;
  transportMethod: string;
}

interface SupplyChainGraphProps {
  productId: string;
  onNodeClick?: (node: SupplyNode) => void;
  onPathHighlight?: (path: SupplyNode[]) => void;
}

const nodeColors = {
  supplier: "#10B981",     // Green
  manufacturer: "#8B5CF6", // Purple
  distributor: "#3B82F6",  // Blue
  retailer: "#EC4899"      // Pink
};

const nodeEmojis = {
  supplier: "🌱",
  manufacturer: "🏭",
  distributor: "🚚",
  retailer: "🏪"
};

const funMessages = {
  sustainable: ["Eco-friendly vibes! 🌿", "Planet-positive! 🌍", "Future-forward! 🚀"],
  verified: ["Verified and vibing! ✅", "Trust built! 🤝", "Transparency wins! 🎯"],
  efficient: ["Supply chain goals! 📦", "Smooth operations! ⚡", "Peak performance! 💫"]
};

export function EnhancedSupplyChainGraph({ productId, onNodeClick, onPathHighlight }: SupplyChainGraphProps) {
  const networkRef = useRef<Network | null>(null);
  const containerRef = useRef<HTMLDivElement>(null);
  const fetcher = useFetcher();
  
  useEffect(() => {
    fetcher.load(`/api/supply-chain/${productId}`);
  }, [productId]);

  useEffect(() => {
    if (!fetcher.data || !containerRef.current) return;

    const nodes = new DataSet(fetcher.data.nodes.map((node: SupplyNode) => ({
      ...node,
      size: node.sustainabilityScore * 10,
      color: {
        background: nodeColors[node.type],
        border: node.verificationStatus === 'verified' ? '#00ff00' : '#ff0000'
      },
      mass: node.type === 'supplier' ? 2 : 1,
      fixed: node.type === 'retailer'
    })));

    const edges = new DataSet(fetcher.data.links.map((link: SupplyLink) => ({
      ...link,
      width: Math.log(link.carbonEmission),
      smooth: {
        type: 'curvedCW',
        roundness: 0.2
      },
      length: link.distance / 100,
      springLength: link.type === 'transport' ? 200 : 100,
      springConstant: 0.04
    })));

    const options: Options = {
      physics: {
        enabled: true,
        barnesHut: {
          gravitationalConstant: -2000,
          centralGravity: 0.3,
          springLength: 95,
          springConstant: 0.04,
          damping: 0.09
        }
      },
      layout: {
        hierarchical: {
          direction: 'LR',
          sortMethod: 'directed',
          levelSeparation: 300
        }
      }
    };

    networkRef.current = new Network(
      containerRef.current,
      { nodes, edges },
      options
    );

    networkRef.current.on('selectNode', (params) => {
      const nodeId = params.nodes[0];
      const node = nodes.get(nodeId);
      onNodeClick?.(node);
      highlightSupplyPath(node);
    });

  }, [fetcher.data]);

  return (
    <div className="p-6 bg-white rounded-xl shadow-lg">
      <h2 className="text-2xl font-bold mb-6">Supply Chain Visualization ✨</h2>
      
      <motion.div
        ref={containerRef}
        className="w-full h-[600px] rounded-lg bg-gray-50"
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        transition={{ duration: 0.5 }}
      />

      <AnimatePresence>
        {fetcher.data?.selectedNode && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: 20 }}
            className="fixed inset-0 bg-black/50 flex items-center justify-center p-4 z-50"
          >
            <motion.div
              className="bg-white rounded-xl p-6 max-w-md w-full"
              onClick={e => e.stopPropagation()}
            >
              {/* Node details modal content */}
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}
