# MeshRenderer

Loads an OBJ mesh from disk and renders it each frame using a specified material. The mesh is looked up relative to `assets/meshes/`, and the material is instantiated via the `Registry<Material>` using the `type` field. Submits the mesh to the renderer using the world matrix from the sibling `Transform`.

**PropertyRef Dependencies:** Requires a sibling `Transform`.

---

## JSON

```json
{
  "properties": {
    "MeshRenderer": {
      "mesh": "my_model.obj",
      "material": {
        "type": "FlatMaterial",
        "color": [1.0, 0.5, 0.2]
      }
    }
  }
}
```

`mesh` is relative to `assets/meshes/`. `material.type` must match a registered material name (e.g. `FlatMaterial`, `LitMaterial`, `EmissiveMaterial`). Additional material fields vary by type.
