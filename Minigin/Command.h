#pragma once

namespace Rinigin {
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
		int GetMaxInputs() const { return m_MaxCallsPerFrame; }
	protected:
		void SetMaxInputs(int max) { m_MaxCallsPerFrame = max; };
	private:
		int m_MaxCallsPerFrame = -1;
	};
}

