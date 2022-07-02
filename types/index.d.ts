export interface ProfilingData {
  title: string;
  startTime: number;
  endTime: number;
  topDownRoot: ProfileNode;
}

export interface ProfileNode {
  name: string;
  scriptName: string;
  scriptId: number;
  columnNumber: number;
  hitCount: number;
  children: ProfileNode[];
}

declare function startProfiling(name?: string): void;
declare function stopProfiling(name?: string): ProfilingData;

export {
  startProfiling,
  stopProfiling
}
