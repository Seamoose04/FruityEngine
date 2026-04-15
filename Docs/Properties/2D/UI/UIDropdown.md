# UIDropdown

A collapsible container with two JSON-defined child regions: a clickable header bar and a toggleable body. The bar is instantiated as a `UIButton` and the body as a `UIContainer`; clicking the bar toggles the body's active state. When closed, only the bar is measured and arranged. When open, the body is laid out directly below the bar within the padded inner rect. Extends `UIContainer`.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (inherited from `UIWidget`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "percent", "value": 100 },
      "height": { "mode": "auto" }
    },
    "UIDropdown": {
      "open": false,
      "bar": {
        "type": "GameObject",
        "name": "dropdown_bar",
        "properties": {
          "UILayout": { "width": { "mode": "percent", "value": 100 }, "height": { "mode": "pixels", "value": 32 } },
          "UIButton": {}
        }
      },
      "body": {
        "type": "GameObject",
        "name": "dropdown_body",
        "properties": {
          "UILayout": { "width": { "mode": "percent", "value": 100 }, "height": { "mode": "auto" } },
          "UIPanel": { "flow": "Vertical", "gap": 4 }
        }
      },
      "padding": { "top": 0, "bottom": 0, "left": 0, "right": 0 },
      "zIndex": 0
    }
  }
}
```

`bar` and `body` are required — each is a full GameObject JSON chunk that will be instantiated as children. The bar chunk must contain a `UIButton` property; the body chunk must contain a `UIContainer`-based property (e.g. `UIPanel`). `open` defaults to `false`. `padding` is inherited from `UIContainer`. `zIndex` is inherited from `UIWidget`.
