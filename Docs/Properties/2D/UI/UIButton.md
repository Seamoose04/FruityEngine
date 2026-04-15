# UIButton

A concrete interactive container that wires `UIClickable`'s callbacks to `ActionRegistry` actions via JSON. Each of the four events — `onClick`, `onRelease`, `onEnter`, `onExit` — can be bound to an `ActionCall` in the JSON definition. Callbacks can also be wired programmatically via `Callback::Subscribe` after instantiation. Extends `UIClickable` — it renders nothing itself; child widgets (labels, panels, icons) provide the visuals.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (inherited from `UIWidget`). Registers with the nearest ancestor `UICanvas` or parent `UIWidget` automatically on creation.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "percent", "value": 100 },
      "height": { "mode": "pixels",  "value": 40 }
    },
    "UIButton": {
      "padding": { "top": 4, "bottom": 4, "left": 8, "right": 8 },
      "onClick":   { "action": "MyAction", "args": { "key": "value" } },
      "onRelease": { "action": "MyAction" },
      "onEnter":   { "action": "MyAction" },
      "onExit":    { "action": "MyAction" },
      "zIndex": 0
    }
  }
}
```

`onClick`, `onRelease`, `onEnter`, and `onExit` are all optional. Each takes an `ActionCall` object with an `action` key and optional `args`. `padding` is inherited from `UIContainer`. `zIndex` is inherited from `UIWidget`.
