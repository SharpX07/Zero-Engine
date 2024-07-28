#pragma once

#include <string>

namespace Zero
{
	class Resource
	{
	public:
		virtual ~Resource() = default;
		virtual std::string GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
	protected:
		std::string m_Name;
	};
}