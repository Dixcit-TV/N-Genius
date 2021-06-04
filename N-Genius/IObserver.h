#pragma once
#include <string>

#include "Enum.h"

namespace ngenius
{
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		IObserver(const IObserver&) = default;
		IObserver(IObserver&&) noexcept = default;
		IObserver& operator=(const IObserver&) = default;
		IObserver& operator=(IObserver&&) noexcept = default;
		
		virtual void OnNotify(const std::string& message, void* pPayload) = 0;
	};
}
