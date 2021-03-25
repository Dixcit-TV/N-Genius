#pragma once
#include "Enum.h"

namespace ngenius
{
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(MessageType event, void* pPayload) = 0;
	};
}
