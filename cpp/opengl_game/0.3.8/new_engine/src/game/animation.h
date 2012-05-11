#ifndef __ANIMATION_H_
#define  __ANIMATION_H_




struct Animation
{
	USHORT frameCount;			// ���������� ������ � ��������
	USHORT startFrame;			// ����� ���������� �����

	USHORT animationSpeed;			// ������ ����� ������ � �������������

	// ���������� ������ �������� ���� ������������ ��������������
	USHORT realX;
	USHORT realY;
	
	// ��� ������������� ��������
	USHORT realMirrorX;
	USHORT realMirrorY;

	// ������ � ������ ������������ ��������������
	USHORT realWidth;
	USHORT realHeight;

	Animation()
	{
		frameCount = startFrame = animationSpeed = realX = realY = realMirrorX = realMirrorY = realWidth = realHeight = 0; // ��� ������ �������, ���� ������ �����.
	}
};

#endif // __ANIMATION_H_