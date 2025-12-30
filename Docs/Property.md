## Property System Overview

The engine uses a **composition-based** design:
A `GameObject` owns multiple `Property` instances.
Each `Property` encapsulates behavior, state, or rendering logic.
Together they form the entity’s complete functionality.

---

### Lifecycle Overview

#### Scene Loading Order

1. **FromJSON phase**

   * The engine parses the scene’s `.json` file.
   * For each `GameObject`, it loads all its `properties` **in order of appearance**.
   * Each property’s `FromJSON(const json& data, const std::string& scenePath)` is called immediately after construction, with `data` set to the inner `"data"` field.
   * Children are recursively loaded after their parent.

2. **OnCreate phase**

   * Once all `GameObjects` and their properties exist:

     * Each `GameObject` calls `OnCreate()` on all its properties.
     * Order matches the JSON definition order.
   * Use this phase to **resolve sibling references** via `PropertyRef`.

3. **Runtime loop**

   ```
   HandleInput(window, dt)
   Update(dt)
   Render(renderer)
   ```

   Each call propagates through all `GameObjects` and their `Properties` in scene order.

4. **OnDestroy phase**

   * Called when a `GameObject` is removed.
   * Runs in **reverse order** (deepest children first).

---

### Creating a Custom Property

1. **Define the class**

   ```cpp
   #pragma once
   #include "game/Property.h"
   #include "util/PropertyRef.h"

   class MyCustomProperty : public Property {
   public:
       void FromJSON(const json& data, const std::string& scenePath) override;
       void OnCreate() override;
       void Update(float dt) override;
       void HandleInput(const Window& window, float dt) override;
       void Render(Renderer& renderer) override;

   private:
       PropertyRef<Transform> _transform; // example dependency
       float speed = 1.0f;
   };
   ```

2. **Implement behavior**

   ```cpp
   void MyCustomProperty::FromJSON(const json& data, const std::string& scenePath) {
       if (data.contains("speed"))
           speed = data["speed"];
   }

   void MyCustomProperty::OnCreate() {
       _transform.SetParent(_parent);
       _transform.Resolve(); // find Transform on same GameObject
   }

   void MyCustomProperty::Update(float dt) {
       if (_transform)
           _transform->position.x += speed * dt;
   }
   ```

3. **Register it**

   ```cpp
   REGISTER_PROPERTY(MyCustomProperty);
   ```

---

### JSON Format Example

Here’s the **correct layout** expected by the scene loader:

```json
{
  "type": "GameObject",
  "properties": [
    {
      "type": "Transform",
      "data": {
        "position": [0, 0, -10],
        "rotation": [0, 0, 0],
        "scale": [1, 1, 1]
      }
    },
    {
      "type": "MeshRenderer",
      "data": {
        "mesh": "meshes/cube.obj",
        "shader": {
          "vertex": "shaders/basic.vert",
          "fragment": "shaders/basic.frag"
        }
      }
    },
    {
      "type": "Rotator",
      "data": {
        "xRot": 20,
        "yRot": 20
      }
    }
  ]
}
```

Each element under `"properties"` corresponds to a class registered with `REGISTER_PROPERTY()`.
The `FromJSON()` method receives the inner `"data"` object.

---

### PropertyRef System

`PropertyRef<T>` allows **safe, lazy access** to other properties on the same `GameObject`.

Typical pattern:

```cpp
PropertyRef<Transform> _transform;

void ExampleProperty::OnCreate() {
    _transform.SetParent(_parent);
    _transform.Resolve();  // Find Transform in same GameObject
}
```

Internally:

* Stores a `weak_ptr<GameObject>` to `_parent`.
* `Resolve()` calls `parent->GetProperty<T>()` and caches the pointer.
* Can be used like a raw pointer: `_transform->position`.
* Logs helpful errors if the target property is missing or the parent expired.

---

### Lifecycle Guidelines

| Stage           | Use for                                                           |
| --------------- | ----------------------------------------------------------------- |
| **Constructor** | Basic initialization only. No references to other properties yet. |
| **FromJSON()**  | Load configuration and external resources.                        |
| **OnCreate()**  | Resolve sibling dependencies, perform runtime setup.              |
| **Update()**    | Frame-by-frame logic.                                             |
| **Render()**    | Drawing, submitting to renderer.                                  |
| **OnDestroy()** | Cleanup and release resources.                                    |

---

### Common Pitfalls

| Pitfall                                        | Why                                            |
| ---------------------------------------------- | ---------------------------------------------- |
| Accessing sibling properties before `OnCreate` | They don’t exist yet.                          |
| Forgetting `REGISTER_PROPERTY()`               | The loader won’t recognize it.                 |
| Ignoring failed `Resolve()`                    | Missing dependencies can silently break logic. |
| Using raw `GameObject` access                  | Prefer `PropertyRef` to maintain modularity.   |

---

### Example: CameraProperty

```json
{
  "type": "CameraProperty",
  "data": {
    "fov": 60.0,
    "near": 0.1,
    "far": 100.0
  }
}
```

In `OnCreate()`, `CameraProperty` would resolve its `Transform` reference, then in `Update()` it builds the view matrix from that transform.
