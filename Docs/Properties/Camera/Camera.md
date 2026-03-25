# Camera

Defines the scene's view and projection, registering itself as the active camera on creation. Supports both `perspective` (with a horizontal FOV) and `orthographic` modes. Each frame it pushes its view matrix and position to the renderer via the sibling `Transform`.

**PropertyRef Dependencies:** Requires a sibling `Transform`.

---

## JSON — Perspective

```json
{
  "properties": {
    "Camera": {
      "mode": "perspective",
      "fov":  90.0,
      "near": 0.1,
      "far":  1000.0
    }
  }
}
```

## JSON — Orthographic

```json
{
  "properties": {
    "Camera": {
      "mode": "orthographic",
      "near": 0.1,
      "far":  1000.0
    }
  }
}
```

`fov` is horizontal FOV in degrees and is only used in `perspective` mode. Screen dimensions for the orthographic projection are set automatically from the renderer.
