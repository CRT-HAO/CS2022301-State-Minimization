import { Button, Textarea } from "@nextui-org/react";
import { useCallback, useState } from "react";
import Module from "./assets/state_minimization.js";
import Markdown from "react-markdown";
import remarkGfm from "remark-gfm";
import "./style.css";
import rehypeRaw from "rehype-raw";

function App() {
  const [kiss, setKiss] = useState("");
  const [result, setResult] = useState("");

  const callRun = useCallback(
    async (kiss: string) => {
      const mymod = await Module();
      const run = mymod.cwrap("run", "string", ["string"]);
      const r = run(kiss);
      setResult(r);
    },
    [setResult]
  );

  const loadExample = () => {
    const example = `.start_kiss
.i 1
.o 1
.p 14
.s 7
.r a
0 a a 0
1 a b 0
0 b c 0
1 b d 0
0 c a 0
1 c d 0
0 d e 0
1 d f 1
0 e a 0
1 e f 1
0 f g 0
1 f f 1
0 g a 0
1 g f 1
.end_kiss
`;

    setKiss(example);
  };

  const clear = () => {
    setKiss("");
    setResult("");
  };

  return (
    <>
      <div className="text-center p-6">
        <h1 className="text-5xl font-bold p-4">State Minimization</h1>
        <div className="h-4"></div>
        <div className="flex flex-col flex-nowrap justify-center align-middle items-center">
          <Textarea
            label="Input kiss"
            placeholder="Enter your input kiss"
            maxRows={30}
            value={kiss}
            onChange={(e) => setKiss(e.target.value)}
            className="font-mono"
          />
          <div className="h-2"></div>
          <div>
            <Button onClick={loadExample}>Example</Button>
            <div className="inline-block w-2"></div>
            <Button color="primary" onClick={() => callRun(kiss)}>
              Minimize
            </Button>
            <div className="inline-block w-2"></div>
            <Button color="danger" onClick={clear}>
              Clear
            </Button>
          </div>
        </div>
        <div className="h-2"></div>
        <div className="md text-left w-screen overflow-x-scroll">
          <Markdown remarkPlugins={[remarkGfm]} rehypePlugins={[rehypeRaw]}>
            {result}
          </Markdown>
        </div>
        <div className="h-12"></div>
        <div className="text-center">
          <p>
            Powered by{" "}
            <a
              href="https://webassembly.org/"
              target="_blank"
              className="text-blue-600"
            >
              WebAssembly
            </a>{" "}
            technology
          </p>
          <p>
            Made by{" "}
            <a
              href="https://github.com/CRT-HAO"
              target="_blank"
              className="text-blue-600"
            >
              HAO
            </a>{" "}
            with ❤️
          </p>
        </div>
      </div>
    </>
  );
}

export default App;
