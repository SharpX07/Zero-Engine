#pragma once

#include <chrono>

namespace Zero
{
	class Timer
	{
	public:
		void Start()
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
			m_IsRunning = true;
		}
		void Stop()
		{
			m_EndTime = std::chrono::high_resolution_clock::now();
			m_IsRunning = false;
		}
		bool IsRunning() const { return m_IsRunning; }
		float GetElapsedTime() const
		{
			if (m_IsRunning)
				return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
			else
				return std::chrono::duration<float>(m_EndTime - m_StartTime).count();
		}
		float GetElapsedTimeMs() const
		{
			return GetElapsedTime() * 1000.0f;
		}
		float GetElapsedTimeS() const
		{
			return GetElapsedTime();
		}
		float GetElapsedTimeMin() const
		{
			return GetElapsedTime() / 60.0f;
		}
		void Reset() { Start(); }

	private:
		std::chrono::high_resolution_clock::time_point m_StartTime;
		std::chrono::high_resolution_clock::time_point m_EndTime;
		bool m_IsRunning;
	};
}