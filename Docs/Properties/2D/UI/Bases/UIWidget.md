# UIWidget

The abstract base for all UI properties. It handles dirty-flag layout, size resolution (pixels / percent / auto), z-index ordering, and automatic registration with the nearest ancestor `UICanvas` or parent `UIContainer`. On creation it walks up the parent chain — if it finds a `UIContainer` first it registers as a child; it keeps walking until it finds a `UICanvas` and caches that too. Subclasses must implement `Draw` and `_Arrange`.

**PropertyRef Dependencies:** Requires a sibling `UILayout` (read via `PropertyRef`). Not directly instantiable — serves as the base class for `UIPanel`, `UILabel`, `UIContainer`, etc.

---

## JSON

```json
{
  "properties": {
    "UILayout": { "..." : "..." },
    "SomeConcreteWidget": {
      "zIndex": 0
    }
  }
}
```

`zIndex` is optional (default `0`). It is read by `UIWidget::FromJSON` and inherited by all subclasses. A sibling `UILayout` must be present on the same GameObject.
