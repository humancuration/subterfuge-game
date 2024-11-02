import React, { useEffect, useRef } from 'react';
import { Network } from 'vis-network';
import { motion } from 'framer-motion';
import type { NetworkNode, NetworkLink } from '~/types/network';

interface NetworkGraphProps {
  nodes: NetworkNode[];
  edges: NetworkLink[];
  onNodeClick?: (nodeId: string) => void;
  onEdgeClick?: (edgeId: string) => void;
  selectedNode?: string | null;
  height?: string;
  config?: {
    nodeSize?: (node: NetworkNode) => number;
    nodeColor?: (node: NetworkNode) => string;
    edgeWidth?: (edge: NetworkLink) => number;
    edgeColor?: (edge: NetworkLink) => string;
    physics?: boolean;
    darkMode?: boolean;
  };
}

export function NetworkGraph({
  nodes,
  edges,
  onNodeClick,
  onEdgeClick,
  selectedNode,
  height = '500px',
  config = {}
}: NetworkGraphProps) {
  const containerRef = useRef<HTMLDivElement>(null);
  const networkRef = useRef<Network | null>(null);

  useEffect(() => {
    if (!containerRef.current) return;

    const defaultOptions = {
      nodes: {
        shape: 'dot',
        size: 16,
        font: {
          size: 12,
          color: config.darkMode ? '#ffffff' : '#000000'
        },
        borderWidth: 2,
        shadow: true,
        scaling: {
          min: 16,
          max: 32,
          label: {
            enabled: true,
            min: 12,
            max: 24
          }
        }
      },
      edges: {
        width: 2,
        color: { 
          inherit: 'both',
          opacity: 0.7
        },
        smooth: {
          type: 'continuous',
          roundness: 0.5
        },
        shadow: true
      },
      physics: {
        enabled: config.physics ?? true,
        barnesHut: {
          gravitationalConstant: -80000,
          springConstant: 0.001,
          springLength: 200,
          damping: 0.09
        },
        stabilization: {
          enabled: true,
          iterations: 1000,
          updateInterval: 50
        }
      },
      interaction: {
        hover: true,
        tooltipDelay: 200,
        zoomView: true,
        dragView: true,
        hideEdgesOnDrag: true
      }
    };

    // Transform nodes and edges to vis-network format
    const visNodes = nodes.map(node => ({
      id: node.id,
      label: node.name,
      value: node.influence,
      group: node.group,
      size: config.nodeSize?.(node) || defaultOptions.nodes.size,
      color: {
        background: config.nodeColor?.(node) || '#6366F1',
        border: selectedNode === node.id ? '#3B82F6' : '#4B5563',
        highlight: {
          background: '#818CF8',
          border: '#3B82F6'
        }
      }
    }));

    const visEdges = edges.map(edge => ({
      from: edge.source,
      to: edge.target,
      width: config.edgeWidth?.(edge) || edge.strength * 2,
      color: {
        color: config.edgeColor?.(edge) || '#9CA3AF',
        highlight: '#3B82F6',
        hover: '#6366F1'
      },
      smooth: {
        type: 'continuous'
      }
    }));

    networkRef.current = new Network(
      containerRef.current,
      { nodes: visNodes, edges: visEdges },
      { ...defaultOptions }
    );

    // Event handlers
    if (onNodeClick) {
      networkRef.current.on('selectNode', params => {
        if (params.nodes.length > 0) {
          onNodeClick(params.nodes[0]);
        }
      });
    }

    if (onEdgeClick) {
      networkRef.current.on('selectEdge', params => {
        if (params.edges.length > 0) {
          onEdgeClick(params.edges[0]);
        }
      });
    }

    // Cleanup
    return () => {
      if (networkRef.current) {
        networkRef.current.destroy();
      }
    };
  }, [nodes, edges, selectedNode, config]);

  return (
    <motion.div
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      className={`bg-white dark:bg-gray-800 rounded-lg shadow-lg overflow-hidden ${
        config.darkMode ? 'dark' : ''
      }`}
    >
      <div 
        ref={containerRef} 
        style={{ height, width: '100%' }}
        className="network-graph"
      />
    </motion.div>
  );
}
