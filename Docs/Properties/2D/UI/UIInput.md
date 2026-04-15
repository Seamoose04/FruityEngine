# UIInput

A focusable text input field built on top of `UIClickable`. On creation it programmatically instantiates its own internal widget tree: a `UIPanel` background, a `UILabel` for the displayed text, a `UIIcon` cursor bar, and an `InputHandler` property for keyboard capture. Clicking the input requests focus from the `UICanvas`; focus toggles the cursor visibility, swaps the background color, and routes key events through `InputHandler`. Exposes `onChange` and `onCommit` callbacks. Implements `IFocusable`.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (inherited from `UIWidget`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "pixels", "value": 200 },
      "height": { "mode": "pixels", "value": 32 }
    },
    "UIInput": {
      "text":      "initial value",
      "font":      "assets/fonts/my_font.json",
      "fontSize":  18.0,
      "color":     [0.6, 0.6, 0.6, 1.0],
      "textColor": [0.0, 0.0, 0.0, 1.0],
      "textAlign": "Start",
      "padding":   { "top": 4, "bottom": 4, "left": 8, "right": 8 },
      "zIndex":    0
    }
  }
}
```

`font` and `fontSize` are required. `text` defaults to `""`. `color` is the background RGBA and defaults to `[0.6, 0.6, 0.6, 1.0]`. `textColor` defaults to opaque black. `textAlign` can be `"Start"`, `"Center"`, or `"End"` and defaults to `"Start"`. `padding` is inherited from `UIContainer`. `zIndex` is inherited from `UIWidget`. Callbacks (`onChange`, `onCommit`) are wired in code, not JSON.
