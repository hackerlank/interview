#ifndef __ANIMATION_H_
#define  __ANIMATION_H_




struct Animation
{
	USHORT frameCount;			// Количество кадров в анимации
	USHORT startFrame;			// Номер начального кадра

	USHORT animationSpeed;			// Период смены кадров в миллисекундах

	// Координаты левого верхнего угла описывающего прямоугольника
	USHORT realX;
	USHORT realY;
	
	// Для отзеркаленной текстуры
	USHORT realMirrorX;
	USHORT realMirrorY;

	// Ширина и высота описывающего прямоугольника
	USHORT realWidth;
	USHORT realHeight;

	Animation()
	{
		frameCount = startFrame = animationSpeed = realX = realY = realMirrorX = realMirrorY = realWidth = realHeight = 0; // Кто досюда дочитал, тому делать нехуй.
	}
};

#endif // __ANIMATION_H_