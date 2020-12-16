#pragma once
#include "hound/core/rendering/target/render_target.h"

class window;

class viewport : public render_target
{
public:	
	viewport() = default;
	virtual ~viewport() = default;
	
protected:
	void begin_frame() override;
	void end_frame() override;
};
