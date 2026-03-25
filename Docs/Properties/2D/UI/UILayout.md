# UILayout

Stores the sizing and spacing rules for a UI widget — width, height, padding, margin, and self-alignment. It does not drive layout itself; rather, it is read by sibling `UIWidget`-based properties during the arrange pass. After layout is resolved, the computed screen-space rectangle is stored here and accessed by the widget for rendering.

**PropertyRef Dependencies:** Required by all `UIWidget`-based properties (`UIPanel`, `UILabel`). They acquire it via `PropertyRef` on the same GameObject.

---

## JSON

Sizes can be specified in three modes:
- `{ "mode": "pixels", "value": 200 }` — fixed pixel size
- `{ "mode": "percent", "value": 50 }` — percentage of available space
- `{ "mode": "auto" }` — size to content

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "percent", "value": 100 },
      "height": { "mode": "pixels",  "value": 48 },
      "padding": { "top": 8, "bottom": 8, "left": 12, "right": 12 },
      "margin":  { "top": 4, "bottom": 4, "left": 0,  "right": 0  },
      "alignSelf": "Center"
    }
  }
}
```

`width` and `height` are required. `padding`, `margin`, and `alignSelf` are optional and default to zero / `Start`.
