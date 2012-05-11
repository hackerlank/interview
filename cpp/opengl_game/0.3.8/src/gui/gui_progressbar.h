// ����� ������������
// GUIProgressBar (GUIVidget)
// ����������:
//	
// --------------------------------------------------------------------------
class GUIProgressBar : public GUIControl
{
protected:
	// ��� - ���� ������� ������������
	Texture* step;

	// ���������� ����� ������ (�� ������������)
	UINT step_margin;

	// ������ ���� (�� ������������)
	UINT step_width;

	// ������ ���� (�� ������������)
	UINT step_height;

	// ��������� ���������� X ������� ����
	UINT step_start_x;

	// ��������� ���������� Y ������� ����
	UINT step_start_y;

	// ���������� ����� - "�����" ������������
	UINT step_count;

	// ������� ���
	UINT current_step;

	// ���� ���� (�������� � current_step)
	UINT step_increment;

public:
	Texture* GetStep();
	UINT GetStepMargin();
	UINT GetStepWidth();
	UINT GetStepHeight();
	UINT GetStepStartX();
	UINT GetStepStartY();
	UINT GetStepCount();
	UINT GetCurrentStep();
	UINT GetStepIncrement();

	void SetStep(Texture* new_step);
	void SetStepMargin(UINT new_margin);
	void SetStepWidth(UINT new_width);
	void SetStepHeight(UINT new_height);
	void SetStepStartX(UINT new_step_start_x);
	void SetStepStartY(UINT new_step_start_y);
	void SetStepCount(UINT new_step_count);
	void SetCurrentStep(UINT new_current_step);
	void SetStepIncrement(UINT new_step_increment);

	// �����������
	GUIProgressBar(const char* name);
};
// --------------------------------------------------------------------------