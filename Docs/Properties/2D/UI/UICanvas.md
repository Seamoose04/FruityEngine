# UICanvas

The root of the UI hierarchy — it owns the screen-space layout pass and the draw call for all registered widgets. On creation it captures the screen size from the renderer; when marked dirty it re-arranges all top-level widgets to fill the full screen. All `UIWidget`-based properties in the scene walk up their parent chain to find and register with the nearest `UICanvas`.

**PropertyRef Dependencies:** None. Widgets register themselves automatically.

---

## JSON

```json
{
  "properties": {
    "UICanvas": {}
  }
}
```

`UICanvas` has no configurable JSON fields. It should live on a dedicated root GameObject that is an ancestor of all UI widget objects.
