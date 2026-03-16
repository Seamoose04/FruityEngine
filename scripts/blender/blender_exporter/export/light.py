# ---------------------------------------------------------------------------
# Light export — stub
# ---------------------------------------------------------------------------
# To implement when the engine gains a Light property:
#
#   def build_light_json(obj) -> dict:
#       light = obj.data
#       transform = build_transform_json(obj, use_local=False)
#       return {
#           "type": "GameObject",
#           "properties": {
#               "Transform": transform,
#               "Light": {
#                   "type":      light.type,        # POINT, SUN, SPOT, AREA
#                   "color":     list(light.color),
#                   "intensity": light.energy,
#               },
#           },
#       }
#
# Then in operators.py, add LIGHT to the object type collection loop
# and call build_light_json for each.
