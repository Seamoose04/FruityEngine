# PostProcessVolume

Defines and wires a post-processing effect graph, registering it with the renderer on first render. Each effect in the `effects` array is identified by an `id` and connected to others via named input slots (`input`, `inputA`, `inputB`, `inputC`). The graph must contain exactly one `Scene` node (the source) and one `RenderToScreen` node (the output).

**PropertyRef Dependencies:** None.

---

## JSON

```json
{
  "properties": {
    "PostProcessVolume": {
      "effects": [
        { "id": "scene",    "type": "Scene" },
        { "id": "bloom",    "type": "BloomThreshold",  "input": "scene",  "threshold": 1.0 },
        { "id": "blurH",    "type": "GaussianBlurH",   "input": "bloom" },
        { "id": "blurV",    "type": "GaussianBlurV",   "input": "blurH" },
        { "id": "combine",  "type": "BloomCombine",    "inputA": "scene", "inputB": "blurV" },
        { "id": "output",   "type": "RenderToScreen",  "input": "combine" }
      ]
    }
  }
}
```

Every effect must have a unique `id` and a registered `type`. Input slots reference other effects by their `id`. Additional fields (e.g. `threshold`) are effect-specific and documented separately.
