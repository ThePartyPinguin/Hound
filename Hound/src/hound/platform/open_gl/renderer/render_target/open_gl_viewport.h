#pragma once
#include "hound/core/rendering/target/viewport.h"

class open_gl_viewport : public viewport
{
private:
	void begin_frame() override;
	void end_frame() override;
};

