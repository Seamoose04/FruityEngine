# Transform

Stores and manages the position, rotation, and scale of a GameObject in 3D space. Supports both local and world-space values, automatically propagating changes down the parent hierarchy using a dirty-flag pull model. All setters mark the transform dirty; matrices are recomputed at the start of the next frame.

**PropertyRef Dependencies:** None.

---

## JSON

```json
{
  "properties": {
    "Transform": {
      "position": [0.0, 1.0, 0.0],
      "rotation": [0.0, 45.0, 0.0],
      "scale":    [1.0, 1.0, 1.0]
    }
  }
}
```

All three fields are optional. Omitted fields default to `[0, 0, 0]` for position/rotation and `[1, 1, 1]` for scale. Values are Euler angles in degrees.
