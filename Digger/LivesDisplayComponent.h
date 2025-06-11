#pragma once
#include <Component.h>

class LivesComponent;
class LivesDisplayComponent : public Rinigin::Component
{
public:
	explicit LivesDisplayComponent(Rinigin::GameObject* gameObject,LivesComponent* livesComponent);
	void Render() const override;
private:
	LivesComponent* m_LivesComponent;
};

