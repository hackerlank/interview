// ����� ������
// GUIButton (GUIControl)
// ����������:
//	������ ������
// --------------------------------------------------------------------------
class GUIButton : public GUIControl
{
public:
	void OnLeftClick();
	void OnMouseMove();

	// �����������
	GUIButton();
	GUIButton(string name);
};
// --------------------------------------------------------------------------