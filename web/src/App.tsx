import { Button, Textarea } from "@nextui-org/react";
import { useCallback, useState } from "react";
import Module from "./assets/state_minimization.js";
import Markdown from "react-markdown";
import remarkGfm from "remark-gfm";
import "./style.css";
import rehypeRaw from "rehype-raw";
import { exampleKiss } from "./example_kiss.js";
import ChangelogModal from "./ChangelogModal.js";
import githubLogo from "./assets/github.svg";

function App() {
  const [kiss, setKiss] = useState("");
  const [result, setResult] = useState("");
  const [changelogModal, setChangelogModal] = useState(true);

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
    setKiss(exampleKiss);
  };

  const clear = () => {
    setKiss("");
    setResult("");
  };

  return (
    <>
      <ChangelogModal
        isOpen={changelogModal}
        onOpenChange={(open) => setChangelogModal(open)}
      />
      <div className="text-center p-6">
        <h1 className="text-5xl font-bold p-4">State Minimization</h1>
        <div className="h-4"></div>
        <div className="flex flex-col flex-nowrap justify-center align-middle items-center">
          <div className="h-4"></div>
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
          <div className="h-4" />
          <p>
            <a
              href="https://github.com/CRT-HAO/CS2022301-State-Minimization"
              target="_blank"
            >
              <img src={githubLogo} className="w-10 inline-block" />
            </a>
            <br />
            Version 0.1.0
          </p>
        </div>
      </div>
    </>
  );
}

export default App;
