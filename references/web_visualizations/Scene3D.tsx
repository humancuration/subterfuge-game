import { useRef } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, Environment, useGLTF } from '@react-three/drei';
import { motion } from 'framer-motion-3d';
import { Vector3 } from 'three';

interface ModelProps {
  path: string;
  position?: [number, number, number];
  scale?: [number, number, number];
  rotation?: [number, number, number];
}

function Model({ path, position = [0, 0, 0], scale = [1, 1, 1], rotation = [0, 0, 0] }: ModelProps) {
  const mesh = useRef<THREE.Mesh>(null);
  const { scene } = useGLTF(path);
  
  useFrame(() => {
    if (mesh.current) {
      mesh.current.rotation.y += 0.01;
    }
  });

  return (
    <primitive 
      ref={mesh}
      object={scene} 
      position={position}
      scale={scale}
      rotation={rotation}
    />
  );
}

interface Scene3DProps {
  models: ModelProps[];
  environmentPreset?: string;
  cameraPosition?: Vector3;
}

export default function Scene3D({ 
  models, 
  environmentPreset = 'sunset',
  cameraPosition = new Vector3(0, 0, 5)
}: Scene3DProps) {
  return (
    <div className="w-full h-[500px] rounded-lg overflow-hidden">
      <Canvas camera={{ position: cameraPosition }}>
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        
        {models.map((model, index) => (
          <Model key={index} {...model} />
        ))}
        
        <Environment preset={environmentPreset} />
        <OrbitControls />
      </Canvas>
    </div>
  );
}
