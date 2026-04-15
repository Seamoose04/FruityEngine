# UIIcon

Renders a flat-colored quad at its computed layout rect, useful for solid-color indicators, dividers, or tintable rectangular elements. Uses `FlatMaterial` and rebuilds its mesh lazily when the layout is re-arranged. Extends `UIWidget` directly — it is a leaf widget with no children.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (read via `PropertyRef`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "pixels", "value": 16 },
      "height": { "mode": "pixels", "value": 16 }
    },
    "UIIcon": {
      "color":  [1.0, 1.0, 1.0, 1.0],
      "zIndex": 1
    }
  }
}
```

`color` is RGBA in `[0–1]` and defaults to opaque white. `zIndex` is inherited from `UIWidget` and is optional (default `0`).
