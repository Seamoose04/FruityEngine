# UIClickable

An invisible interactive container that provides mouse hover and click detection over its computed layout rect. It exposes four `Callback` signals — `onClick`, `onEnter`, `onExit`, `onRelease` — which can be wired programmatically via `Callback::Subscribe` or through `UIButton`'s JSON action bindings. Extends `UIContainer` — it arranges children into its padded inner rect and measures content as the max extents of its active children. Renders nothing itself; child widgets (labels, panels, icons) provide the visuals.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (inherited from `UIWidget`). Typically used as the base class for `UIButton` rather than instantiated directly.

---

## JSON

```json
{
  "properties": {
    "UILayout": {
      "width":  { "mode": "percent", "value": 100 },
      "height": { "mode": "pixels",  "value": 40 }
    },
    "UIClickable": {
      "padding": { "top": 4, "bottom": 4, "left": 8, "right": 8 },
      "zIndex": 0
    }
  }
}
```

No additional JSON fields beyond those inherited from `UIContainer` (`padding`) and `UIWidget` (`zIndex`). Callbacks are wired in code, not JSON.
