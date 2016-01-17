/*
Author: Shane Coates
Description: FlyCamera class
Inherits from Camera base class
*/

#ifndef _SPINNING_CAMERA_H_
#define	_SPINNING_CAMERA_H_

#include <Camera.h>
struct GLFWwindow;
class SpinningCamera : public Camera
{
public:
	SpinningCamera() : Camera()
	{
		Init();
	}
	SpinningCamera(float _flySpeed) : Camera(), m_flySpeed(_flySpeed)
	{
		Init();
	}
	~SpinningCamera() {};

	void Init();

	virtual void Update(double _dt);

	void SetFlySpeed(float _flySpeed)		{ m_flySpeed = _flySpeed; }
	float GetFlySpeed()						{ return m_flySpeed; }

protected:
	void HandleKeyboardInput(double _dt);

	float m_flySpeed;
};

#endif