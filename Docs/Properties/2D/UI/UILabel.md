# UILabel

Renders a string of text using a bitmap font atlas, positioning glyphs within its computed layout rect. Font size, color, and horizontal alignment are all configurable. Extends `UIWidget` — all `UIWidget` JSON fields apply and are read automatically.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (read via `PropertyRef`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "auto" },
      "height": { "mode": "auto" }
    },
    "UILabel": {
      "text":     "Hello World",
      "font":     "assets/fonts/my_font.json",
      "fontSize": 24.0,
      "color":    [1.0, 1.0, 1.0, 1.0],
      "hAlign":   "Center",
      "zIndex":   1
    }
  }
}
```

`text`, `font`, and `fontSize` are required. `color` defaults to opaque black. `hAlign` can be `"Start"`, `"Center"`, or `"End"` and defaults to `"Start"`. `zIndex` is inherited from `UIWidget` and is optional (default `0`).
