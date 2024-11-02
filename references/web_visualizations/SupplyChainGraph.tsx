import { useEffect, useRef, useState } from 'react';
import * as d3 from 'd3';
import { motion } from 'framer-motion';
import type { SupplyChainNode, SupplyChainLink } from '~/services/visualization.server';

interface SupplyChainGraphProps {
  nodes: SupplyChainNode[];
  links: SupplyChainLink[];
  onNodeClick?: (node: SupplyChainNode) => void;
  onPathHighlight?: (path: SupplyChainNode[]) => void;
}

export default function SupplyChainGraph({
  nodes,
  links,
  onNodeClick,
  onPathHighlight
}: SupplyChainGraphProps) {
  const svgRef = useRef<SVGSVGElement>(null);
  const [selectedPath, setSelectedPath] = useState<string[]>([]);

  useEffect(() => {
    if (!svgRef.current) return;

    const width = 1200;
    const height = 800;
    const svg = d3.select(svgRef.current);

    svg.selectAll('*').remove();

    const simulation = d3.forceSimulation<SupplyChainNode>(nodes)
      .force('link', d3.forceLink<SupplyChainNode, SupplyChainLink>(links)
        .id(d => d.id)
        .distance(link => calculateLinkDistance(link)))
      .force('charge', d3.forceManyBody().strength(-1000))
      .force('center', d3.forceCenter(width / 2, height / 2))
      .force('collision', d3.forceCollide().radius(50));

    const container = svg.append('g');
    
    const zoom = d3.zoom<SVGSVGElement, unknown>()
      .scaleExtent([0.1, 4])
      .on('zoom', (event) => {
        container.attr('transform', event.transform.toString());
      });

    svg.call(zoom);

    const links = container.append('g')
      .selectAll('path')
      .data(links)
      .join('path')
      .attr('class', 'supply-chain-link')
      .attr('stroke-width', link => Math.sqrt(link.carbonEmission))
      .attr('stroke', link => getColorByVerificationStatus(link.verificationStatus));

    const nodes = container.append('g')
      .selectAll('g')
      .data(nodes)
      .join('g')
      .attr('class', 'supply-chain-node')
      .call(d3.drag<SVGGElement, SupplyChainNode>()
        .on('start', dragStarted)
        .on('drag', dragged)
        .on('end', dragEnded));

    nodes.append('circle')
      .attr('r', node => getNodeRadius(node))
      .attr('fill', node => getNodeColor(node))
      .attr('stroke', '#fff')
      .attr('stroke-width', 2);

    nodes.append('text')
      .text(d => d.name)
      .attr('dy', 30)
      .attr('text-anchor', 'middle');

    simulation.on('tick', () => {
      links.attr('d', generateLinkPath);
      nodes.attr('transform', d => `translate(${d.x},${d.y})`);
    });

    nodes.on('click', (event, node) => {
      const path = findCompletePath(node, links);
      setSelectedPath(path.map(n => n.id));
      if (onNodeClick) onNodeClick(node);
      if (onPathHighlight) onPathHighlight(path);
    });

    return () => {
      simulation.stop();
    };
  }, [nodes, links, onNodeClick, onPathHighlight]);

  return (
    <div className="relative w-full">
      <svg
        ref={svgRef}
        className="w-full h-[800px]"
        viewBox="0 0 1200 800"
      />
      {selectedPath.length > 0 && (
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          className="absolute bottom-4 right-4 bg-white p-4 rounded-lg shadow-lg"
        >
          <h3 className="font-bold mb-2">Selected Path</h3>
          <div className="space-y-1">
            {selectedPath.map(nodeId => {
              const node = nodes.find(n => n.id === nodeId);
              return (
                <div key={nodeId} className="text-sm">
                  {node?.name}
                </div>
              );
            })}
          </div>
        </motion.div>
      )}
    </div>
  );
}

// Helper functions
function calculateLinkDistance(link: SupplyChainLink) {
  return 200;
}

function getColorByVerificationStatus(status: string) {
  switch (status) {
    case 'verified': return '#22c55e';
    case 'pending': return '#eab308';
    case 'disputed': return '#ef4444';
    default: return '#94a3b8';
  }
}

function getNodeRadius(node: SupplyChainNode) {
  return 20 + (node.sustainabilityScore * 10);
}

function getNodeColor(node: SupplyChainNode) {
  switch (node.type) {
    case 'supplier': return '#3b82f6';
    case 'manufacturer': return '#8b5cf6';
    case 'distributor': return '#ec4899';
    case 'retailer': return '#f97316';
    default: return '#94a3b8';
  }
}

function generateLinkPath(d: any) {
  return `M${d.source.x},${d.source.y}L${d.target.x},${d.target.y}`;
}

function findCompletePath(node: SupplyChainNode, links: SupplyChainLink[]) {
  // Implementation of path finding algorithm
  return [node];
}

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
