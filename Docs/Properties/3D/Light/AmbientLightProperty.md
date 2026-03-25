# AmbientLightProperty

Submits a global ambient light to the renderer each frame, uniformly affecting all lit surfaces in the scene. Only one ambient light is expected to be active at a time. There are no positional properties — it applies scene-wide.

**PropertyRef Dependencies:** None.

---

## JSON

```json
{
  "properties": {
    "AmbientLightProperty": {
      "color":     [1.0, 1.0, 1.0],
      "intensity": 0.2
    }
  }
}
```

`color` is an RGB triplet in the range `[0–1]`. `intensity` scales the color and can exceed `1.0` for HDR scenes.
