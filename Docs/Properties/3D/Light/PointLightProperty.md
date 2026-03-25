# PointLightProperty

Emits light outward in all directions from the GameObject's world position, submitted to the renderer's light queue each frame. Position is sourced automatically from the sibling `Transform` — it does not need to be set in JSON. Multiple point lights can exist in a scene up to the renderer's UBO limit.

**PropertyRef Dependencies:** Requires a sibling `Transform`.

---

## JSON

```json
{
  "properties": {
    "PointLightProperty": {
      "color":     [1.0, 0.4, 0.1],
      "intensity": 3.0,
      "radius":    15.0
    }
  }
}
```

`color` is an RGB triplet. `intensity` can exceed `1.0` for HDR bloom contribution. `radius` is the world-space falloff radius of the light.
