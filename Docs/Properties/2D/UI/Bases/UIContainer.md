# UIContainer

An abstract container that owns a list of child `UIWidget`s and draws them in order. It adds padding support on top of `UIWidget` and provides the `ActiveChildren()` filtered accessor, which returns only children whose GameObjects are currently active. Subclasses (e.g. `UIPanel`, `UIClickable`) implement `MeasureContent` and extend `_Arrange` to position children within the padded inner rect. The base `_Arrange` marks all active children dirty so they re-layout on the next pass.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (inherited from `UIWidget`). Not directly instantiable — serves as the base class for `UIPanel`, `UIClickable`, `UIButton`, etc.

---

## JSON

```json
{
  "properties": {
    "UILayout": { "..." : "..." },
    "SomeConcreteContainer": {
      "padding": { "top": 8, "bottom": 8, "left": 12, "right": 12 },
      "zIndex": 0
    }
  }
}
```

`padding` is optional and defaults to zero on all sides. `zIndex` is inherited from `UIWidget`.
