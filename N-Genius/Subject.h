#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "Enum.h"
#include "IObserver.h"

namespace ngenius
{
	class Subject
	{
	public:
		virtual ~Subject() = default;

		void AddObserver(std::weak_ptr<IObserver> pNewObserver) { m_Observers.push_back(std::move(pNewObserver)); }

		void Notify(Event event, void* pPayload)
		{
			bool needCleanup{ false };

			std::for_each(std::begin(m_Observers), std::end(m_Observers), [event, pPayload, &needCleanup](std::weak_ptr<IObserver>& pObserver)
			{
				if (!pObserver.expired())
					pObserver.lock()->OnNotify(event, pPayload);
				else
					needCleanup = true;
			});

			if (needCleanup)
			{
				m_Observers.erase(std::remove_if(std::begin(m_Observers), std::end(m_Observers), [](std::weak_ptr<IObserver>& pObserver)
					{
						return pObserver.expired();
					})
					, std::end(m_Observers));
			}
		}

	private:
		std::vector<std::weak_ptr<IObserver>> m_Observers;
	};
}