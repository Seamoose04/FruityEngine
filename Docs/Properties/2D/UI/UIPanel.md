# UIPanel

A rectangular UI container that arranges its child widgets either vertically, horizontally, or stacked by depth. It renders a flat-colored quad at its computed rect and respects padding when laying out children. Extends `UIWidget` — all `UIWidget` JSON fields apply and are read automatically.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (read via `PropertyRef`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "percent", "value": 100 },
      "height": { "mode": "percent", "value": 100 },
      "padding": { "top": 10, "bottom": 10, "left": 10, "right": 10 }
    },
    "UIPanel": {
      "color": [0.1, 0.1, 0.1, 0.9],
      "flow":  "Vertical",
      "gap":   8.0,
      "zIndex": 0
    }
  }
}
```

`color` is RGBA in `[0–1]` and defaults to transparent. `flow` can be `"Vertical"`, `"Horizontal"`, or `"Depth"` (stacked). `gap` is the pixel spacing between children. `zIndex` is inherited from `UIWidget` and is optional (default `0`).
