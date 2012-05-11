// Класс прогрессбара
// GUIProgressBar (GUIVidget)
// Примечание:
//	
// --------------------------------------------------------------------------
class GUIProgressBar : public GUIControl
{
protected:
	// Шаг - один элемент прогрессбара
	Texture* step;

	// Расстояние между шагами (на прогрессбаре)
	UINT step_margin;

	// Ширина шага (на прогрессбаре)
	UINT step_width;

	// Высота шага (на прогрессбаре)
	UINT step_height;

	// Стартовая координата X первого шага
	UINT step_start_x;

	// Стартовая координата Y первого шага
	UINT step_start_y;

	// Количество шагов - "длина" прогрессбара
	UINT step_count;

	// Текущий шаг
	UINT current_step;

	// Сила шага (прибавка к current_step)
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

	// Конструктор
	GUIProgressBar(const char* name);
};
// --------------------------------------------------------------------------