#include "game/Property.h"

class HierarchyController : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void OnDestroy() override;

	const std::string& GetSelectedFile() const;
	const std::string& GetSelectedNode() const;

private:
	void _SelectNode(const std::string& file, const std::string& node);

	std::string _selectedFile;
	std::string _selectedNode;
};
