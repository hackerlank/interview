// Класс контейнера
// GUIContainer (GUIBaseControl)
// Примечание:
//	Просто контейнер, на основе его будет сделана панелька.
//	Только контейнер может содержать чайлдов
// --------------------------------------------------------------------------
class GUIContainer : public GUIBaseControl
{
protected:
	// Дети контейнера (любой объект)
	vector<GUIBaseControl*> children;

	// Количество детей
	UINT child_count;

public:
	UINT GetChildCount();
	GUIBaseControl* GetControlByName(char* name);
	GUIBaseControl* GetControlByGID(GID id);
	
	void AddChild(GUIBaseControl* child);
	void DeleteChildByName(char* name);
	void SetChildCount(UINT new_child_count);
	void Clear();
	void Show();
	void Hide();

	// Конструктор
	GUIContainer(const char* name);
};
// --------------------------------------------------------------------------