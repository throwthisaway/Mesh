#pragma once
namespace Scene
{
	class IItem
	{
	public:
		virtual void Draw(void) = 0;
		virtual void Cleanup(void) = 0;
	};
}
